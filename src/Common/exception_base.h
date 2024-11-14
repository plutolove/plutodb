
#include <stdexcept>
#include <string>

class ExceptionBase : public std::exception {
 public:
  ExceptionBase(const std::string& msg, int code = 0);

  ExceptionBase(const std::string& detail_msg, const std::string& msg,
                int code = 0);

  ExceptionBase(const std::string& msg, const ExceptionBase& nested,
                int code = 0);

  ExceptionBase(const ExceptionBase& exc);

  ~ExceptionBase() throw();

  ExceptionBase& operator=(const ExceptionBase& exc);

  virtual const char* name() const throw();

  virtual const char* className() const throw();

  virtual const char* what() const throw();

  const ExceptionBase* nested() const;

  const std::string& message() const;

  int code() const;

  std::string displayText() const;

  virtual ExceptionBase* clone() const;

  virtual void rethrow() const;

 protected:
  ExceptionBase(int code = 0);

  void message(const std::string& msg);

  void extendedMessage(const std::string& arg);

 private:
  std::string _msg;
  ExceptionBase* _pNested;
  int _code;
};

inline const ExceptionBase* ExceptionBase::nested() const { return _pNested; }

inline const std::string& ExceptionBase::message() const { return _msg; }

inline void ExceptionBase::message(const std::string& msg) { _msg = msg; }

inline int ExceptionBase::code() const { return _code; }
