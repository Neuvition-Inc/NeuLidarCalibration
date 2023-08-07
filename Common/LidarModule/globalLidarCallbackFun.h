#pragma once

#include <neuv_defs.hpp>



void g_cb_on_framedata(int code, int64_t microsec, const neuvition::NeuvUnits& data, const neuvition::nvid_t& frame_id);
void g_cb_on_mjpgdata(int code, int64_t microsec, cv::Mat mat);
void g_cb_on_imudata(int code, int64_t microsec, const neuvition::NeuvUnits &data, const neuvition::ImuData &imu);