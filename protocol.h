/*
  算法与电控通信协议 V1.1（20260309）
*/

#pragma once
#include <stdint.h>

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#define FRAME_SOF 0xF6         // 帧头
#define MAX_SIZE_SERIAL_CV 64  //不建议一包大于64字节

typedef struct Frame {
  uint8_t sof;
  uint8_t data_len;
  uint8_t data[MAX_SIZE_SERIAL_CV];
} Frame_t;

#define AUTOAIM_MCU2AI 0x5A  // 电控 -> 视觉 (自瞄用)数据包头
/* 电控 -> 视觉 (自瞄用)MCU数据结构体*/
struct Protocol_MCUPacket_t {
  uint8_t header = AUTOAIM_MCU2AI;
  uint8_t detect_color : 1;  // 0-red 1-blue
  uint8_t task_mode : 2;     // 0-aim 1-buff+aim
  bool reset_tracker : 1;
  uint8_t reserved : 2;
  float roll;
  float pitch;
  float yaw;
  float aim_x;
  float aim_y;
  float aim_z;
  bool shoot_flag_debug;  // debug
  uint16_t game_time;  // (s) game time [0, 450]
  uint32_t timestamp;  // (ms) board time
  uint16_t checksum = 0;
} __attribute__((packed));

#define AUTOAIM_AI2MCU 0xA5  // 视觉 -> 电控 (自瞄用)数据包头
/* 视觉 -> 电控 (自瞄用)数据结构体*/
struct Protocol_MasterPacket_t {
  uint8_t header = AUTOAIM_AI2MCU;
  uint8_t state : 2;       // 0-untracking 1-tracking-aim 2-tracking-buff
  uint8_t id : 3;          // aim: 0-outpost 6-guard 7-base
  uint8_t armors_num : 3;  // 2-balance 3-outpost 4-normal
  float x;                 // aim: robot-center || buff: rune-center
  float y;                 // aim: robot-center || buff: rune-center
  float z;                 // aim: robot-center || buff: rune-center
  float yaw;               // aim: robot-yaw || buff: rune-theta  // spd = a*sin(w*t)+b || spd > 0 ==> clockwise
  float vx;  // aim: robot-vx || buff: rune spin speed param - a
  float vy;  // aim: robot-vy || buff: rune spin speed param - b
  float vz;  // aim: robot-vz || buff: rune spin speed param - w
  float v_yaw;
  float r1;
  float r2;
  float dz;
  float letency_time;
  uint32_t cap_timestamp;  // (ms) frame capture time
  uint16_t t_offset;       // (ms) speed t offset
  uint16_t checksum = 0;
} __attribute__((packed));

#define DECISION_MCU2AI 0x6A  // 决策数据包头
/* 电控 -> 视觉 (决策用)裁判系统数据结构体*/
struct Protocol_UpDataReferee_t {
  uint8_t header = DECISION_MCU2AI;
  uint8_t game_progress;                  /* 当前比赛阶段 */
  uint16_t stage_remain_time;             /* 当前阶段剩余时间 */
  uint16_t robot_1_hp;                /* 1英雄机器人血量 */
  uint16_t robot_2_hp;                /* 2工程机器人血量 */
  uint16_t robot_3_hp;                /* 3步兵机器人血量 */
  uint16_t robot_4_hp;                /* 4步兵机器人血量 */
  uint16_t robot_7_hp;                /* 7哨兵机器人血量 */
  uint16_t outpost_hp;                /* 己方前哨站血量 */
  uint16_t base_hp;                   /* 己方基地血量 */

  uint8_t robot_id;                       /* 本机器人ID（1~7->红，101~107->蓝）*/
  uint16_t current_hp;                    /* 机器人当前血量 */
  uint16_t maximum_hp;                    /* 机器人血量上限 */
  uint16_t shooter_17_mm_barrel_heat;   /* 17mm发射机构的射击热量 */
  uint16_t projectile_allowance_17mm;     /* 17mm弹丸允许发弹量 */
  uint16_t remaining_gold_coin;           /* 剩余金币数量 */
  uint32_t center_gain_point;             /* 中心增益点的占领状态(仅RMUL适用) */
  bool team_color;                        /* 队伍颜色 0->红 1->蓝 */
  uint8_t decision_num;                   /* 选择决策模式 */
  //以下为26赛季新增内容
  uint16_t exchange_ammo_amount_17mm;     /* 17mm弹丸已兑换数量 */
  uint16_t exchange_ammo_succes_count_17mm;  /* 17mm弹丸已兑换成功次数 */
  uint16_t exchange_hp_succes_count;      /*血量兑换成功次数 */
  bool free_revive;                       /* 是否可以免费复活 */
  bool paid_revive;                       /* 是否可以付费立即复活 */
  uint16_t paid_revive_cost;              /* 当前付费复活消耗金币数 */
  bool out_of_combat;                     /* 是否处于脱战状态 */
  uint16_t team_exchange_ammo_left_17mm;  /* 队伍17mm弹丸剩余可兑换数量*/
  uint8_t current_stance;                 /* 哨兵当前姿态，1为进攻姿态，2为防御姿态，3为移动姿态 */
  bool can_activate_mechanism;           /* 己方能量机关是否能够进入正在激活状态，1 为当前可激活 */
  uint16_t checksum = 0;
} __attribute__((packed));


#define DECISION_AI2MCU 0xB6  // 自主决策行为数据包头
/* 决策 -> 电控  哨兵自主决策信息 结构体 */
struct Protocol_SentryCommand_t {
  uint8_t  header = DECISION_AI2MCU;
  bool     decide_free_revive;           // 是否复活
  bool     decide_paid_revive ;     // 是否兑换立即复活
  uint16_t exchange_ammo_amount;   //   兑换发弹量 最小100 此值的变化需要单调递增，否则视为不合法。
  uint8_t  remote_ammo_request;     //  兑换弹丸请求次数 此值的变化需要单调递增且每次仅能增加 1，否则视为不合法。
  uint8_t  remote_hp_request;       //  兑换血量请求次数 此值的变化需要单调递增且每次仅能增加 1，否则视为不合法。
  uint8_t  current_stance;                 // 姿态 1为进攻姿态，2为防御姿态，3为移动姿态
  bool     activate_mechanism;       //是否让能量机关激活
  uint8_t  chassis_gyro_mode;       // 底盘陀螺模式: 0=停止, 1=低速, 2=高速
  uint16_t checksum = 0;
} __attribute__((packed));


#define NAVIGATION_AI2MCU 0xA6  // 导航数据包头
/* 视觉 -> 电控 (导航用)数据结构体*/
struct Protocol_NavCommand_t {
  uint8_t header = NAVIGATION_AI2MCU;
  struct __attribute__((packed)) {
    float yaw; /* 偏航角(Yaw angle) */
    float pit; /* 俯仰角(Pitch angle) */
    float rol; /* 翻滚角(Roll angle) */
  } gimbal;    /* 欧拉角 */

  struct __attribute__((packed)) {
    float vx;         /* x轴移动速度 */
    float vy;         /* y轴移动速度*/
    float wz;         /* z轴转动速度 */
  } chassis_move_vec; /* 底盘移动向量 */
  uint16_t checksum = 0;
} __attribute__((packed));

#define OMNIANGLE_AI2MCU 0xA7  // 全向角数据包头
/* 视觉 -> 电控 (全向角数据) 结构体 */
struct Protocol_OmniAngle_t {
  uint8_t header = OMNIANGLE_AI2MCU;

  struct __attribute__((packed)) {
    float yaw;   /* 全向角Yaw角度 */
    float pitch; /* 全向角Pitch角度 */
  } angle;       /* 全向角信息 */

  uint16_t checksum = 0; /* 校验和 */
} __attribute__((packed));

#ifdef __cplusplus
}
#endif
