namespace ExpParser {

  using namespace std;

  class ParseException: public exception
  {
  };

  class EvalException: public exception
  {
  };

  template<typename T>
  class member {
  public:
    string name;
    virtual T* eval(vector<T*>& parameters);
  };

  class Value {
  public:
    virtual string tostring() = 0;
    virtual int get_int() = 0;
    vector<member<Value>*> members;
  };

  Value* voidunit();

  template<typename T>
  class ValueAny: public Value {
  public:
    T value;
    string s;
  public:
    //    Kind kind;
    ValueAny(T v, string pp){
      value = v;
      s = pp;
    }
    virtual string tostring(){
      return s;
    }
    virtual int get_int(){
      return 0;
    }
  };

  class FValue {
  public:
    string name;
    virtual Value* eval(vector<Value*>& parameters);
  };

  class Env {
  public:
    vector<string> names;
    vector<Value*> values;
    vector<FValue*> functions;
    void add(string name, Value* value);
  };

  ValueAny<double>* doubleValue(Value* p);

  ValueAny<int>* intValue(Value* p);

  Env* newenv();

  enum ExprKind { ExFunc, ExComp };

  class Func;

  class Expr {
  public:
    ExprKind kind;
    virtual unique_ptr<Expr> copy()=0;
    virtual unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values)=0;
    virtual unique_ptr<Func> FuncMe()=0;
    virtual Value* eval(Env& env)=0;
    virtual string tostring()=0;
  };

  unique_ptr<ExpParser::Expr> load(int argc, char **argv);
}
