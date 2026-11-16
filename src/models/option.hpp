enum class OptionType { Call, Put };
enum class ExerciseStyle { European, American };

struct Option {
  OptionType type;
  double strike;
  double maturity; //in years
  ExerciseStyle style = ExerciseStyle::European;
};

struct MarketData {
  double spot;
  double rate;
  double volatility;
};


