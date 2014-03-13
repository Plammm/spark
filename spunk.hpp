#ifndef Spunk_version
#define Spunk_version 1

namespace Spunk {

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
    virtual Value* add(member<Value>* m) = 0;
  };

  Value* voidunit();

  template<typename T>
  class ValueAny: public Value {
  public:
    T value;
    string s;
  public:
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
    Value* add(member<Value>* m){
      members.push_back(m);
      return this;
    }
  };

  class FValue {
  public:
    string name;
    virtual Value* eval(vector<Value*>& parameters);
  };


  struct envValue {
    string name;
    Value* value;
    envValue(string name, Value* value){
      this->name = name;
      this->value = value;
    }
  };

  class Env {
  public:
    vector<envValue*> values;
    vector<FValue*> functions;
    void add(string name, Value* value){
      values.push_back(new envValue(name, value));
    }
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
    virtual Value* eval(Env& env)=0;
    virtual string tostring()=0;
  };

  unique_ptr<Expr> load(int argc, char **argv);
}

#endif
