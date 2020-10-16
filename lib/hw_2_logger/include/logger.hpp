#pragma once

#include <fstream>
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

} // namespace log
