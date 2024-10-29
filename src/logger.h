class Logger {
private:
  bool _active;
public:
  void enable() { _active = true; };
  void disable() { _active = false; };
};
