/*
  视觉与电控通信协议
*/

#pragma once
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/* 电控 -> 视觉 (自瞄用)MCU数据结构体*/
struct Protocol_MCUPacket_t {
  uint8_t header = 0x5A;
  uint8_t detect_color : 1; // 0-red 1-blue
  bool reset_tracker : 1;
  uint8_t reserved : 6;
  float roll;
  float pitch;
  float yaw;
  float aim_x;
  float aim_y;
  float aim_z;
  uint16_t checksum = 0;
} __attribute__((packed));

/* 视觉 -> 电控 (自瞄用)数据结构体*/
struct Protocol_MasterPacket_t {
  uint8_t header = 0xA5;
  bool tracking : 1;
  uint8_t id : 3;         // 0-outpost 6-guard 7-base
  uint8_t armors_num : 3; // 2-balance 3-outpost 4-normal
  uint8_t reserved : 1;
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
  uint16_t team;         /* 本身队伍 */
  uint16_t time;         /* 比赛开始时间 */
  uint8_t race;          /* 比赛类型 */
  uint8_t rfid;          /* 增益地点 */
  uint8_t base_hp;       /* 基地血量 */
  uint8_t sentry_hp;     /* 哨兵血量 */
  uint8_t ballet_remain; /* 剩余弹量 */
  uint8_t arm;           /* 兵种信息 */
  uint16_t checksum = 0;
} __attribute__((packed));

/* 视觉 -> 电控 (导航用)数据结构体*/
struct Protocol_NavCommand_t {
  uint8_t header = 0xA6;
  struct __attribute__((packed)) {
    float yaw; /* 偏航角(Yaw angle) */
    float pit; /* 俯仰角(Pitch angle) */
    float rol; /* 翻滚角(Roll angle) */
  } gimbal;    /* 欧拉角 */

  struct __attribute__((packed)) {
    float vx; /* x轴移动速度 */
    float vy; /* y轴移动速度、哨兵沿轨道方向(正面面向战场，右为正方向) */
    float wz; /* z轴转动速度、哨兵射界(弧度0～pi) */
  } chassis_move_vec; /* 底盘移动向量 */
  uint16_t checksum = 0;
} __attribute__((packed));

#ifdef __cplusplus
}
#endif