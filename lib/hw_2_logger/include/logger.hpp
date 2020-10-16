#pragma once

#include <string>
#include <fstream>

namespace log{

enum class Level{
  DEBUG,
  INFO,
  WARN,
  ERROR
};

std::string LevelToString(const Level& level);

class BaseLogger {
public:
  virtual void Log(const Level& level, const std::string& msg);
  virtual void Debug(const std::string& msg);
  virtual void Info(const std::string& msg);
  virtual void Warn(const std::string& msg);
  virtual void Error(const std::string& msg);
  void SetLevel(const Level& level) {level_ = level;}
  Level GetCurrentLevel(){return level_;}
  virtual void Flush(){}

private:
  Level level_ = Level::INFO;
  virtual void PrintLog(const Level& level, const std::string& msg){}
};

class StdoutLogger : public BaseLogger{
private:
  void Flush() override;
  void PrintLog(const Level& level, const std::string& msg) override;
};

class StderrLogger : public BaseLogger{
private:

  void Flush() override;
  void PrintLog(const Level& level, const std::string& msg) override;
};


class FileLogger : public BaseLogger {
  public:
  FileLogger(const std::string& filename);
  ~FileLogger();
private:
  std::ofstream log_file_;
  void Flush() override;
  void PrintLog(const Level& level, const std::string& msg) override;
};

}
