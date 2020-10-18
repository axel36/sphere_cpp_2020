#pragma once

#include <fstream>
#include <memory>
#include <string>

namespace log {

enum class Level { DEBUG, INFO, WARN, ERROR };

class BaseLogger {
public:
  void Log(Level level, const std::string &msg);
  void Debug(const std::string &msg);
  void Info(const std::string &msg);
  void Warn(const std::string &msg);
  void Error(const std::string &msg);
  void SetLevel(Level level);
  Level GetCurrentLevel();
  virtual void Flush();
  virtual ~BaseLogger() = default;

private:
  Level level_ = Level::INFO;
  virtual void PrintLog(Level level, const std::string &msg) = 0;

protected:
  void PrintLogInternal(std::ostream &stream, Level level,
                        const std::string &msg);
};

class StdoutLogger : public BaseLogger {
private:
  void PrintLog(Level level, const std::string &msg) override;
};

class StderrLogger : public BaseLogger {
private:
  void PrintLog(Level level, const std::string &msg) override;
};

class FileLogger : public BaseLogger {
public:
  explicit FileLogger(const std::string &filename);
  ~FileLogger();

private:
  std::ofstream log_file_;
  void Flush() override;
  void PrintLog(Level level, const std::string &msg) override;
};

class LoggerSingleton {
public:
  static LoggerSingleton &GetInstance();

  void SetGlobalLogger(std::unique_ptr<BaseLogger> &logger);
  BaseLogger &GetGlobalLogger();

private:
  LoggerSingleton() = default;
  LoggerSingleton(const LoggerSingleton &root) = delete;
  LoggerSingleton &operator=(const LoggerSingleton &) = delete;

  std::unique_ptr<BaseLogger> global_logger_ = std::make_unique<StdoutLogger>();
};

void init_with_stdout_logger(Level level);
void init_with_stderr_logger(Level level);
void init_with_file_logger(Level level, const std::string &file_path);

void DEBUG(const std::string &msg);
void INFO(const std::string &msg);
void WARN(const std::string &msg);
void ERROR(const std::string &msg);

} // namespace log
