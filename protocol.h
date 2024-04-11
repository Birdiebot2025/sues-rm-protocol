/*
  视觉与电控通信协议
*/

#pragma once
#include <cstdint>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define CV_COMM_SOF 0xF6 // start of frame

/* 电控 -> 视觉 (自瞄用)MCU数据结构体*/
struct Protocol_MCUPacket_t {
  uint8_t header = 0x5A;
  uint8_t detect_color : 1; // 0-red 1-blue
  bool reset_tracker : 1;   // 0-不重置 1-重置
  uint8_t reserved : 6;     // 保留位
  float roll;
  float pitch;
  float yaw;
  float aim_x; // 火控计算出的目标点
  float aim_y; // 火控计算出的目标点
  float aim_z; // 火控计算出的目标点
  uint16_t checksum = 0;
} __attribute__((packed));

/* 视觉 -> 电控 (自瞄用)数据结构体*/
struct Protocol_MasterPacket_t {
  uint8_t header = 0xA5;
  bool tracking : 1;      // 0-不追踪 1-追踪
  uint8_t id : 3;         // 0-outpost 6-guard 7-base
  uint8_t armors_num : 3; // 2-balance 3-outpost 4-normal
  uint8_t reserved : 1;   // 保留位
  float x;
  float y;
  float z;
  float yaw;
  float vx;
  float vy;
  float vz;
  float v_yaw;
  float r1;
  float r2;
  float dz;
  float letency_time;
  uint16_t checksum = 0;
} __attribute__((packed));

/* 电控 -> 视觉 (导航用)裁判系统数据结构体*/
struct Protocol_UpDataReferee_t {
  uint8_t header = 0x6A;
  uint8_t robot_id;      /*机器人ID （1~7->红，101~107->蓝*/
  uint16_t current_hp;   /*血量*/
  uint16_t shooter_heat; /*枪口热量*/
  bool team_color;       /*队伍颜色 0->红 1->蓝*/
  bool is_attacked; /*是否受到攻击 0->未受到攻击 1->受到攻击*/
  uint8_t game_progress;      /*当前比赛阶段*/
  uint16_t stage_remain_time; /*当前阶段剩余时间*/

  // 场地事件数据
  // bit 0-2：
  // bit 0：己方补给站 1号补血点占领状态 1为已占领；
  // bit 1：己方补给站 2号补血点占领状态 1为已占领；
  // bit 2：己方补给站 3号补血点占领状态 1为已占领；
  // bit 3-5：己方能量机关状态：
  // • bit 3为打击点占领状态，1为占领；
  // • bit 4为小能量机关激活状态，1为已激活；
  // • bit 5为大能量机关激活状态，1为已激活；
  // bit 6：己方侧R2/B2环形高地占领状态1为已占领；
  // bit 7：己方侧R3/B3梯形高地占领状态 1为已占领；
  // bit 8：己方侧R4/B4梯形高地占领状态 1为已占领；
  // bit 9：己方基地护盾状态：
  // • 1为基地有虚拟护盾血量；
  // • 0为基地无虚拟护盾血量；
  // bit 10：己方前哨战状态：
  // • 1为前哨战存活；
  // • 0为前哨战被击毁；
  // bit 10-31: 保留
  uint32_t event_data;

  uint8_t decision_num; /*决策切换 0-xxx 1-xxx 2-xxx ... */
  uint16_t checksum = 0;
} __attribute__((packed));

/* 视觉 -> 电控 (导航用)数据结构体*/
struct Protocol_NavCommand_t {
  uint8_t header = 0xA6;
  struct __attribute__((packed)) {
    float q1; /* 四元数q1 */
    float q2; /* 四元数q2 */
    float q3; /* 四元数q3 */
    float q4; /* 四元数q4 */
  } gimbal;   /* 四元数 */

  struct __attribute__((packed)) {
    float vx;         /* x轴移动速度 */
    float vy;         /* y轴移动速度*/
    float wz;         /* z轴转动速度 */
  } chassis_move_vec; /* 底盘移动向量 */
  uint16_t checksum = 0;
} __attribute__((packed));

#ifdef __cplusplus
}
#endif
