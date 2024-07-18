/*
  视觉与电控通信协议
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
  bool reset_tracker : 1;    // 重置识别器 0-不重置 1-重置
  uint8_t reserved : 6;      // 保留位
  float roll;
  float pitch;
  float yaw;
  float aim_x;  // 火控计算出的目标点
  float aim_y;  // 火控计算出的目标点
  float aim_z;  // 火控计算出的目标点
  uint16_t checksum = 0;
} __attribute__((packed));

#define AUTOAIM_AI2MCU 0xA5  // 视觉 -> 电控 (自瞄用)数据包头
/* 视觉 -> 电控 (自瞄用)数据结构体*/
struct Protocol_MasterPacket_t {
  uint8_t header = AUTOAIM_AI2MCU;
  bool tracking : 1;       // 0-不追踪 1-追踪
  uint8_t id : 3;          // 0-outpost 6-guard 7-base
  uint8_t armors_num : 3;  // 2-balance 3-outpost 4-normal
  uint8_t reserved : 1;    // 保留位
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

#define DECISION_MCU2AI 0x6A  // 决策数据包头
/* 电控 -> 视觉 (决策用)裁判系统数据结构体*/
struct Protocol_UpDataReferee_t {
  uint8_t header = DECISION_MCU2AI;
  uint8_t robot_id;           /*机器人ID （1~7->红，101~107->蓝*/
  uint16_t current_hp;        /*血量*/
  uint16_t shooter_heat;      /*枪口热量*/
  bool team_color;            /*队伍颜色 0->红 1->蓝*/
  uint8_t game_progress;      /*当前比赛阶段*/
  uint16_t stage_remain_time; /*当前阶段剩余时间*/
  uint16_t remaining_bullet;  // Sentry's remaining bullet
  uint16_t red_outpost_hp;
  uint16_t blue_outpost_hp;
  uint8_t decision_num;     /*决策切换 0-xxx 1-xxx 2-xxx ... */
  float target_position_x;  // Aerial operator's sentry target position x
  float target_position_y;  // Aerial operator's sentry target position y
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

#ifdef __cplusplus
}
#endif
