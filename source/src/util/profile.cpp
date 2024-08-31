#include "util/profile.hpp"
#include "util/file_util.hpp"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>


Profile::Profile(const std::string &name) : name(name) {
  start = std::chrono::high_resolution_clock::now();
}

Profile::~Profile() {
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  // 获取当前时间
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);

  // 使用 localtime_s 替代 localtime
  std::tm local_time;
  localtime_s(&local_time, &now_time);

  // 打开日志文件
  std::ofstream log_file(ROOT_DIR + "profile_log.txt", std::ios_base::app);
  if (log_file.is_open()) {
    log_file << "[" << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "] "
             << "Profile \"" << name << "\" took " << duration.count() << "ms"
             << std::endl;
  }

  
}