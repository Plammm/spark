#include <iostream>
#include <cctype>
#include <vector>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <typeinfo>
#include <memory>
#include <boost/filesystem.hpp>
#include <exception>

using namespace std;

#define faileval {cout << "Line: " << __LINE__ << endl; throw new EvalException;}
#define failparse {cout << "Line: " << __LINE__ << " " << context.nextPos << endl; throw new ParseException;}
#define ccout(s) {cout << "Line: " << __LINE__ << " NextPos: " << nextPos << " " << s << endl; }
#define here // {cout << "Here: " << __LINE__ << endl;}
#define check_parameters(siz) if (parameters.size() != siz) faileval;int n = 0;
#define getany(any,v) ValueAny<any>* v ## ptr = dynamic_cast<ValueAny<any>*>(parameters[n]); if (v ## ptr == 0) faileval; any v = v ## ptr->value; n++;
#define getptr(any,v) getany(any*,v)
#define getdouble(v) ValueAny<double>* v ## ddd = doubleValue(parameters[n]); if (v ## ddd == 0) faileval; double v = v ## ddd->value; n++;
#define getint(v) ValueAny<int>* v ## ddd = intValue(parameters[n]); if (v ## ddd == 0) faileval; int v = v ## ddd->value; n++;

namespace Spunk
{
  // void handler(int sig) {

  //   void *array[10];
  //   size_t size;

  //   // get void*'s for all entries on the stack
  //   size = backtrace(array, 10);

  //   // print out all the frames to stderr
  //   fprintf(stderr, "Error: signal %d:\n", sig);
  //   backtrace_symbols_fd(array, size, STDERR_FILENO);
  //   //string[] messages = backtrace_symbols(array, size);

  //   //exit(1);
  // }

  class ParseException: public exception
  {
    public:
    //    ParseException(string message){this.message = message;}
    virtual const char* what() const throw()
    {
      return "My exception happened";
    }
  };

  class EvalException: public exception
  {
    public:
    //    ParseException(string message){this.message = message;}
    virtual const char* what() const throw()
    {
      return "My exception happened";
    }
  };


  template<typename T>
  struct namedValue {
    string name;
    T value;
    namedValue(string name, T value){
      this->name = name;
      this->value = value;
    }
  };

  template<typename T>
  int assoc(vector<namedValue<T>*>& values, string& name){
    for(int i = values.size() - 1; 0 <= i; i--)
      if (values[i]->name == name)
        return i;
    return -1;
  }

  class parsingContext {
  public:
    string s;
    unsigned int nextPos = 0;
    vector<namedValue<int>*> variables;
    parsingContext(string s){
      this->s = s;
      this->nextPos = 0;
    }
    parsingContext(parsingContext& context){
      this->s = context.s;
      this->nextPos = context.nextPos;
      this->variables = context.variables;
    }
    bool hasChar(){
      return nextPos < s.length();
    }
    char get(){
      return s[nextPos];
    }
    bool hasChar(unsigned int len){
      return nextPos + len < s.length();
    }
    char get(unsigned int len){
      return s[nextPos + len];
    }
    int intid(string s){
      int pos = assoc<int>(variables, s);
      if (pos == -1)
	return 0;
      return variables[pos]->value;
    }
    string* id(string s){
      int pos = assoc<int>(variables, s);
      if (pos == -1){
	parsingContext context = *this;
	failparse;
      }
      return string_of_int(variables[pos]->value);
    }
    string* idornot(string& s){
      int pos = assoc<int>(variables, s);
      if (pos == -1){
	return &s;
      }
      return string_of_int(variables[pos]->value);
    }
    string* nextid(string s){
      variables.push_back(new namedValue<int>(s, intid(s) + 1));
      return id(s);
    }
    string* string_of_int(int i){
      char output[1024];
      sprintf(output, "%i", i);
      return new string(output);
    }
  };

  enum tokenType { Ident, StringCst, End, Op, Unknown, Keyword };

  struct token {
    int pos;
    int nextPos;
    string s;
    tokenType type;
  };

  string ops = "+-*/=%";
  string intchars = "-0123456789";
  string doublechars = "-0123456789.";

  bool stringContains(string& s, char c){
    for(unsigned int i = 0; i < s.length(); i++)
      if(c == s[i])
        return true;
    return false;
  }

  bool isint(string& s){
    for(unsigned int i = 0; i < s.length(); i++)
      if (!stringContains(intchars, s[i]))
        return false;
    return true;
  }

  bool isdouble(string& s){
    for(unsigned int i = 0; i < s.length(); i++)
      if (!stringContains(doublechars, s[i]))
        return false;
    return true;
  }

  bool isOp(char c){
    return stringContains(ops, c);
  }

  string spaces = " \t\n\r";

  bool isSpace(char c){
    for(unsigned int i = 0; i < spaces.length(); i++)
      if(c == spaces[i])
        return true;
    return false;
  }

  bool isLetter(char c){
    return isalpha(c) || isalnum(c) || c == '_';
  }

  int identToken(parsingContext& context){
    int len = 0;
    if (!context.hasChar())
      return len;
    if(stringContains(intchars, context.get()))
      while(context.hasChar(len) && stringContains(doublechars, context.get(len)))
        len++;
    else
      while(context.hasChar(len) && isLetter(context.get(len)))
        len++;
    return len;
  }

  bool HasToken(parsingContext& context){
    int length = context.s.length();
    if (0 == length)
      return false;
    unsigned int maxpos = context.s.length() - 1;
    unsigned int pos = context.nextPos;
    while(pos <= maxpos){
      if (pos < maxpos - 1 && context.s[pos] == '/' && context.s[pos + 1] == '/')
        while(pos <= maxpos && context.s[pos] != '\n')
          pos++;
      else if (!isSpace(context.s[pos])){
        return true;
      }
      pos++;
    }
    return false;
  }

  unique_ptr<token> NextToken(parsingContext& context){
    unique_ptr<token> tok(new token);
    int len = 0;
    tokenType type = Unknown;
    bool done = false;
    here;
    while(!done){
      if (!context.hasChar())
        {
          cout << "HALT " << context.nextPos << endl;
	  //          handler(1);
          throw new exception();
          type = End;
          break;
        }
      done = true;
      here;
      switch(context.s[context.nextPos]){
      case '{':
      case '}':
      case '.':
      case ';':
      case '(':
      case ',':
      case ')':
        len = 1;
        type = Keyword;
        break;
      case ' ':
      case '\t':
      case '\n':
      case '\r':
	here;
        context.nextPos++;
        done = false;
        break;
      case '"':
        {
          //      int nextPos = -12;
          //failparse;
          len = 1;
          while(context.nextPos + len < context.s.length() && context.s[context.nextPos + len] != '"')
            len++;
          len++;
          type = StringCst;
        }
        break;
      case '/':
	here;
	if (context.nextPos < context.s.length() - 2 && context.s[context.nextPos + 1] == '/'){
          while(context.nextPos < context.s.length() && context.s[context.nextPos] != '\n')
            context.nextPos++;
          done = false;
	  here;
	}
        else{
          len = 1;
          type = Op;
        }
        break;
      default:
        if(isOp(context.get())){
          len = 1;
          type = Op;
        }
        else{
          len = identToken(context);
          type = Ident;
        }
        break;
      }
    }
    tok->pos = context.nextPos;
    if (type != End)
      tok->s = context.s.substr(context.nextPos, len);
    tok->type = type;
    tok->nextPos = context.nextPos + len;
    if (type == StringCst){
      if (type != End)
        tok->s = context.s.substr(context.nextPos + 1, len - 2);
      //cout << "STRING " << pos << " " << len << " " << tok->s << endl;
    }
    return tok;
  }

  bool HasToken(parsingContext& context, string v){
    parsingContext inner (context);
    if (!HasToken(inner))
      return false;
    here;
    unique_ptr<token> tok = NextToken(inner);
    here;
    //    cout << "TOK:" << tok->s << "|" << endl;
    if (tok->s != v)
      return false;
    here;
    context.nextPos = tok->nextPos;
    return true;
  }

  class Expr;

  string ppexpr(unique_ptr<Expr>& e);

  template<typename T>
  class member {
  public:
    string name;
    virtual T* eval(vector<T*>& parameters) = 0;
  };

  class Value {
  public:
    virtual string tostring() = 0;
    virtual int get_int() = 0;
    vector<member<Value>*> members;
    Value* add(member<Value>* m){
      members.push_back(m);
      return this;
    }
  };

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
  };

  ValueAny<string>* newstring(string s);
  ValueAny<vector<Value*>*>* newvector(vector<Value*>* point);

  template<>
  class ValueAny<int>: public Value {
  public:
    int value;
    string s;
  public:
    ValueAny(int v, string pp){
      value = v;
      s = pp;
    }
    ValueAny(int v){
      char output[1024];
      sprintf(output, "%i", v);
      value = v;
      s = output;
    }
    virtual string tostring(){
      return s;
    }
    virtual int get_int(){
      return value;
    }
  };

  template<>
  class ValueAny<double>: public Value {
  public:
    double value;
    string s;
  public:
    ValueAny(double v, string pp){
      value = v;
      s = pp;
    }
    ValueAny(double v){
      char output[1024];
      sprintf(output, "%g", v);
      value = v;
      s = output;
    }
    virtual string tostring(){
      return s;
    }
    virtual int get_int(){
      return (int) value;
    }
  };

  class ValueF: public Value {
  public:
    unique_ptr<Expr> funcValue;
    vector<string> parameters;
    ValueF(unique_ptr<Expr>& f, vector<string> params){
      funcValue = move(f);
      parameters = params;
    }
    string tostring(){
      string params = "";
      for(unsigned int i = 0; i < parameters.size(); i++){
	params += " " + parameters[i];
      }
      return "fun" + params + " -> " + ppexpr(funcValue);
    }
    virtual int get_int(){
      faileval;
    }
  };

  Value* voidunit() {
    return new ValueAny<int>(0, "unit");
  }

  class FValue {
  public:
    string name;
    virtual Value* eval(vector<Value*>& parameters)=0;
  };

  class Env {
  public:
    vector<namedValue<Value*>*> values;
    vector<FValue*> functions;
    void add(string name, Value* value){
      values.push_back(new namedValue<Value*>(name, value));
    }
  };

  enum ExprKind { ExFunc, ExComp };

  class Func;

  class Expr {
  public:
    ExprKind kind;
    virtual unique_ptr<Expr> copy()=0;
    virtual unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values)=0;
    virtual unique_ptr<Func> FuncMe()=0;
    //    virtual unique_ptr<Expr> simplify()=0;
    virtual Value* eval(Env& env)=0;
    virtual string tostring()=0;
  };

  string ppexpr(unique_ptr<Expr>& e){
    return e->tostring();
  }

  class ValueExpr: public Expr {
  public:
    Value* v;
    ValueExpr(Value* value){
      v = value;
    }
    unique_ptr<Expr> copy(){return unique_ptr<Expr>(new ValueExpr(v));}
    virtual unique_ptr<Func> FuncMe(){
      //      ValueF* f = dynamic_cast<ValueF*>(env.values[i]);
      cout << "FUNCME: " << v->tostring() << endl;
      faileval;
    }
    virtual unique_ptr<Expr> substitute(vector<string>&, vector<unique_ptr<Expr>>&){
      return copy();
    }
    virtual Value* eval(Env&){
      return v;
    }
    virtual string tostring(){
      return v->tostring();
    }
  };

  class StringExpr: public Expr {
  public:
    string v;
    StringExpr(string s){
      v = s;
    }
    virtual unique_ptr<Expr> simplify(){
      return copy();
    }
    unique_ptr<Expr> copy(){return unique_ptr<Expr>(new StringExpr(v));}
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    virtual unique_ptr<Expr> substitute(vector<string>&, vector<unique_ptr<Expr>>&){
      unique_ptr<Expr> result(this);
      return result;
    }
    virtual Value* eval(Env&){
      return newstring(v);
    }
    virtual string tostring(){
      return v;
    }
  };

  class Func: public Expr {
  public:
    string name;
    bool isFunction;
    bool isInfix;
    vector<unique_ptr<Expr>> parameters;
    bool isValue;
    unique_ptr<Func> innerCopy(){
      here;
      unique_ptr<Func> result (new Func);
      result->name = name;
      result->isFunction = isFunction;
      result->isInfix = isInfix;
      for(unsigned int i = 0; i < parameters.size(); i++)
        result->parameters.push_back(parameters[i]->copy());
      result->isValue = isValue;
      here;
      return move(result);
    }
    unique_ptr<Expr> copy(){
      return move(innerCopy());
    }
    Func(){ isFunction=false; isInfix=false; isValue=false;kind=ExFunc; }
    unique_ptr<Expr> applySubstitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      here;
      unique_ptr<Func> result(new Func);
      result->name = name;
      result->isFunction = isFunction;
      result->isInfix = isInfix;
      for(unsigned int i = 0; i < parameters.size(); i++)
        result->parameters.push_back(parameters[i]->substitute(names, values));
      return move(result);
    }
    unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      here;
      //unique_ptr<Expr>* v;// = assoc<string, Expr>(names, values, name));
      int id = -1;
      for(unsigned int i = 0; i < names.size(); i++)
        if (names[i] == name)
          id = i;
      //v = &(values[i]);
      here;
      if (id < 0){
        here;
        if (parameters.size() == 0)
          return copy();
        return applySubstitute(names, values);
      }
      here;
      //      cout << "++ " << id << " " << values.size() << endl;
      //cout << tostring() << " " << name << endl;
      //cout << "values[id] " << values[id]->tostring() << endl;
      unique_ptr<Expr> v = values[id]->copy();
      here;
      //cout << "SUB " << name << " " << parameters.size() << " " << tostring() << " " << v->tostring() <<endl;
      if (!isFunction)
        return v->copy();
      else{
        here;
	ValueExpr* f = dynamic_cast<ValueExpr*>(&(*v));
	if (f == 0)
	  faileval;
	ValueF* g = dynamic_cast<ValueF*>(f->v);
	if (g == 0)
	  faileval;

	//        if (v->kind != ExFunc)
        //  faileval;
        here;
        //Func v = v.get();
        //unique_ptr<Func> vv ((Func*)(&(v*)));
	//        unique_ptr<Func> vv = g->FuncMe();
        // cout << tostring() << endl;
        // cout << v->tostring() << " " << typeid(*v).name() << endl;
        // cout << vv->name << " " << vv->isFunction << " " << vv->parameters.size() << " " << (vv->isValue?"true":"false") << endl;
	//        if (vv->isFunction || vv->isValue)
	// faileval;
	unique_ptr<Expr> funcValue;
	//vector<string> parameters;
	//	cout << "SUBST " << g->parameters.size() << " " << values.size() << endl;
	//	cout << "SUBST " << g->tostring();
	unique_ptr<Expr> result = g->funcValue->substitute(g->parameters, parameters);
	//	unique_ptr<Expr> result = g->funcValue->substitute(g->parameters, values);

	//        here;
	// unique_ptr<Func> result = applySubstitute(names, values)->FuncMe();
        //result->name = vv->name;
        return move(result);
      }
    }
    virtual unique_ptr<Func> FuncMe(){
      return innerCopy();
    }
    Value* eval(Env& env){
      here;
      //cout << name << endl;
      if (isValue)
        faileval;
      //return v;
      if (isFunction) {
        if (name.length() == 0 && parameters.size() == 1)
          return parameters[0]->eval(env);
        here;
        for(unsigned int i = 0; i < env.functions.size(); i++)
          if (name == env.functions[i]->name){
            FValue* f = env.functions[i];
            vector<Value*> params;
            for(unsigned int j = 0; j < parameters.size(); j++)
              params.push_back(parameters[j]->eval(env));
            return f->eval(params);
          }
        here;
        for(int i = env.values.size() -1; 0 <= i; i--)
          if (name == env.values[i]->name){
            ValueF* f = dynamic_cast<ValueF*>(env.values[i]->value);
            if (f == 0)
              faileval;
            if (f->parameters.size() != parameters.size()){
              cout << "Bad nb parameters for application. " << tostring() << endl;
              faileval;
            }
            here;
	    // **********************
            vector<unique_ptr<Expr>> params;
            for(unsigned int j = 0; j < parameters.size(); j++){
	      Value* v = parameters[j]->eval(env);
	      unique_ptr<ValueExpr> param(new ValueExpr(v));
              params.push_back(move(param));
	    }
            unique_ptr<Expr> e = f->funcValue->substitute(f->parameters, params);
            here;
            return e->eval(env);
            //int length =
            faileval;
          }
        cout << "Unknown function: " << name << " ";
        faileval
      }
      here;
      for(int i = env.values.size() -1; 0 <= i; i--)
        if (name == env.values[i]->name)
          return env.values[i]->value;
      // if (stringContains(name, '.')){ // TODO isdouble
      //   float res = 0.;
      //   char* nameChars = (char*)(name.data());
      //   sscanf(nameChars, "%g", &res);
      //   return new ValueAny<double>((double)res, name);
      // } else if (isint(name)){
      //   int res = 0;
      //   char* nameChars = (char*)(name.data());
      //   sscanf(nameChars, "%i", &res);
      //   return new ValueAny<int>(res, name);
      // }
      cout << "Unknown variable: " + name << endl;
      faileval;
    }
    string tostring() {
      string result;
      if (isInfix){
        unsigned int size = parameters.size();
        for(unsigned int i = 0; i < size; i++){
          result += parameters[i]->tostring();
          if (i < size - 1)
            result += " " + name + " ";
        }
        return result;
      }
      result = name;
      if (isFunction)
        result += "(";
      unsigned int size = parameters.size();
      for(unsigned int i = 0; i < size; i++)
        {
          result += parameters[i]->tostring();
          if (i < size - 1)
            result += ", ";
        }
      if (isFunction)
        result += ")";
      return result;
    }
  };

  class Comp: public Expr {
  public:
    unique_ptr<Expr> v;
    string member;
    vector<unique_ptr<Expr>> parameters;
    unique_ptr<Comp> innerCopy(){
      here;
      unique_ptr<Comp> result (new Comp);
      result->member = member;
      result->v = v->copy();
      for(unsigned int i = 0; i < parameters.size(); i++)
        result->parameters.push_back(parameters[i]->copy());
      here;
      return move(result);
    }
    unique_ptr<Expr> copy(){
      return move(innerCopy());
    }
    Comp(){ kind=ExComp; }
    unique_ptr<Expr> applySubstitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      here;
      unique_ptr<Comp> result(new Comp);
      result->member = member;
      result->v = v->substitute(names, values);
      for(unsigned int i = 0; i < parameters.size(); i++)
        result->parameters.push_back(parameters[i]->substitute(names, values));
      return move(result);
    }
    unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      here;
      return applySubstitute(names, values);
      here;
    }
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    Value* eval(Env& env){
      here;
      Value* vv = v->eval(env);
      //cout << "VV " << vv->tostring() << endl;
      for(unsigned int i = 0; i < vv->members.size(); i++){
	//	cout << "MEMBER " << vv->members[i]->name << endl;
        if (member == vv->members[i]->name){
          vector<Value*> params;
          params.push_back(vv);
          for(unsigned int j = 0; j < parameters.size(); j++)
            params.push_back(parameters[j]->eval(env));
          return vv->members[i]->eval(params);
        }
      }
      cout << "Unknown member: " << member << endl;
      faileval;
    }
    string tostring() {
      string result = v->tostring() + "." + member + "(";
      unsigned int size = parameters.size();
      for(unsigned int i = 0; i < size; i++)
        {
          result += parameters[i]->tostring();
          if (i < size - 1)
            result += ", ";
        }
      result += ")";
      return result;
    }
  };

#define new_result(t) unique_ptr<t> result(new t);

  class Definition: public Expr {
  public:
    bool isFunction;
    string name;
    vector<string> parameters;
    unique_ptr<Expr> value;
    Definition(){isFunction=false;}
    unique_ptr<Expr> copy(){
      new_result(Definition);
      result->isFunction = isFunction;
      result->name = name;
      result->parameters = vector<string>(parameters);
      result->value = value->copy();
      return move(result);
    }
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      vector<string> subnames;
      vector<unique_ptr<Expr>> subvalues;
      for(unsigned int i = 0; i < names.size(); i++)
        if (names[i] != name){
          subnames.push_back(names[i]);
          subvalues.push_back(values[i]->copy());
        }
      unique_ptr<Expr> v = value->substitute(subnames, subvalues);
      unique_ptr<Definition> result(new Definition);
      result->isFunction = isFunction;
      result->name = name;
      result->parameters = parameters;
      result->value = move(v);
      return move(result);
    }
    virtual string tostring() { return "var " + name + " = " + value->tostring() + ";"; }
    virtual Value* eval(Env& env){
      Value* result;
      if (isFunction)
        result = new ValueF(value, parameters);
      else
        result = value->eval(env);
      env.values.push_back(new namedValue<Value*>(name, result));
      return voidunit();
    }
  };

  class Assign: public Expr {
  public:
    string name;
    unique_ptr<Expr> value;
    unique_ptr<Expr> copy(){
      new_result(Assign);
      result->name = name;
      result->value = value->copy();
      return move(result);
    }
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    virtual unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      unique_ptr<Assign> result(new Assign);
      result->name = name;
      result->value = value->substitute(names, values);
      return move(result);
    }

    virtual string tostring() { return "set " + name + " = " + value->tostring() + ";"; }
    virtual Value* eval(Env& env){
      //Value* v = value.eval(env);
      //cout << name << " <- " << v->tostring() << endl;
      for(int i = env.values.size() -1; 0 <= i; i--)
        if (name == env.values[i]->name){
          env.values[i]->value = value->eval(env);
          return voidunit();
        }
      cout << "Unknown variable: " + name << endl;
      faileval;
    }
  };

  // class Call: public Expr {
  // public:
  //   Func f;
  //   virtual string tostring() {
  //     return f.tostring() + ";";
  //   }
  //   virtual Value* eval(Env& env){
  //     //cout << "ENV" << env.names.size() << endl;
  //     f.eval(env);
  //     //  int length = env.names.size();
  //     //for(unsigned int i = 0; i < commands.size(); i++)
  //     //     commands[i]->eval(env);
  //     //env.resize(length);
  //   }
  // };

  class Block: public Expr {
  public:
    vector<unique_ptr<Expr>> commands;
    unique_ptr<Expr> copy(){
      new_result(Block);
      for(unsigned int i = 0; i < commands.size(); i++)
        result->commands.push_back(commands[i]->copy());
      return move(result);
    }
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      here;
      unique_ptr<Block> b(new Block);
      //cout << "SIZE " << commands.size() << endl;
      for(unsigned int i = 0; i < commands.size(); i++){
        here;
        //cout << commands[i]->tostring() << endl;
        // here;
        // cout << commands[i]->tostring() << endl;
        // here;
        // cout << commands[i]->substitute(names, values)->tostring() << endl;
        // here;
        // cout << commands[i]->tostring() << endl;
        // here;
        // cout << commands[i]->substitute(names, values)->tostring() << endl;
        // here;
        // cout << commands[i]->substitute(names, values)->tostring() << endl;
        // here;
        b->commands.push_back(commands[i]->substitute(names, values));
        here;
      }
      here;
      return move(b);
    }
    virtual string tostring() {
      string s = "{\n";
      for(unsigned int i = 0; i < commands.size(); i++)
        s += commands[i]->tostring() + "\n";
      s += "}\n";
      return s;
    }
    virtual Value* eval(Env& env){
      here;
      int length = env.values.size();
      here;
      for(unsigned int i = 0; i < commands.size(); i++)
        commands[i]->eval(env);
      env.values.resize(length);
      here;
      return voidunit();
    }
  };

  class For: public Expr {
  public:
    string variable;
    unique_ptr<Expr> seq;
    unique_ptr<Expr> command;
    unique_ptr<Expr> copy(){
      new_result(For);
      result->variable = variable;
      result->seq = seq->copy();
      result->command = command->copy();
      return move(result);
    }
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      unique_ptr<For> result(new For);
      vector<string> subnames;
      vector<unique_ptr<Expr>> subvalues;
      for(unsigned int i = 0; i < names.size(); i++)
        if (names[i] != variable){
          subnames.push_back(names[i]);
          subvalues.push_back(values[i]->copy());
        }
      result->variable = variable;
      result->seq = seq->substitute(subnames, subvalues);
      result->command = command->substitute(subnames, subvalues);
      return move(result);
    }
    virtual string tostring() {
      return "for("+variable+" in "+seq->tostring()+") "+command->tostring();
    }
    virtual Value* eval(Env& env){
      //      Value* seqvalue = seq->eval(env);
      ValueAny<vector<Value*>*>* seqvalue = dynamic_cast<ValueAny<vector<Value*>*>*>(seq->eval(env));
      if (seqvalue == 0)
        faileval;
      vector<Value*>* vect = seqvalue->value;
      int length = env.values.size();
      env.values.push_back(new namedValue<Value*>(variable, voidunit()));
      for(unsigned int i = 0; i < vect->size(); i++)
        {
          env.values[length]->value = (*vect)[i];
          command->eval(env);
        }
      env.values.resize(length);
      return voidunit();
    }
  };

  class While: public Expr {
  public:
    unique_ptr<Expr> cond;
    unique_ptr<Expr> command;
    unique_ptr<Expr> copy(){
      new_result(While);
      result->cond = cond->copy();
      result->command = command->copy();
      return move(result);
    }
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      unique_ptr<While> result(new While);
      result->cond = cond->substitute(names, values);
      result->command = command->substitute(names, values);
      return move(result);
    }
    virtual string tostring() {
      return "while("+cond->tostring()+") "+command->tostring();
    }
    virtual Value* eval(Env& env){
      while(cond->eval(env)->get_int() != 0){
        int length = env.values.size();
        command->eval(env);
        env.values.resize(length);
      }
      return voidunit();
    }
  };

  class Ifte: public Expr {
  public:
    unique_ptr<Expr> cond;
    unique_ptr<Expr> commandt;
    unique_ptr<Expr> commande;
    unique_ptr<Expr> copy(){
      new_result(Ifte);
      result->cond = cond->copy();
      result->commandt = commandt->copy();
      result->commande = commande->copy();
      return move(result);
    }
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      unique_ptr<Ifte>result(new Ifte);
      result->cond = cond->substitute(names, values);
      result->commandt = commandt->substitute(names, values);
      result->commande = commande->substitute(names, values);
      return move(result);
    }
    virtual string tostring() {
      return "if("+cond->tostring()+") "+commandt->tostring() + " else " + commande->tostring();
    }
    virtual Value* eval(Env& env){
      if(cond->eval(env)->get_int() != 0)
        return commandt->eval(env);
      else
        return commande->eval(env);
    }
  };

  class Parser {
  public:
    virtual unique_ptr<Expr> parse(parsingContext& context) = 0;
  };

  class ParserUnaryOps: public Parser{
    string ops;
    Parser* rec;
  public:
    ParserUnaryOps(string ops, Parser* rec){this->ops = ops; this->rec = rec;}
    unique_ptr<Expr> parse(parsingContext& context){
      if (HasToken(context, "-")){
        if (!HasToken(context))
          failparse;
        unique_ptr<Expr> f1 = rec->parse(context);
        unique_ptr<Func> result(new Func);
        result->name = "-";
        result->isFunction = true;
        result->parameters.push_back(move(f1));
        return move(result);
      }
      return rec->parse(context);
    }
  };


  class ParserBinOps: public Parser{
    string ops;
    Parser* rec;
  public:
    ParserBinOps(string ops, Parser* rec){this->ops = ops; this->rec = rec;}
    unique_ptr<Expr> parse(parsingContext& context){
      unique_ptr<Expr> f1 = rec->parse(context);
      while(true){
        if (!HasToken(context))
          return f1;
        unique_ptr<token> tok = NextToken(context);
        if (tok->type == Op && tok->s.length() == 1 && ops.find(tok->s) != string::npos){
          context.nextPos = tok->nextPos;
          unique_ptr<Expr> f2 = rec->parse(context);
          unique_ptr<Func> result(new Func);
          result->isInfix = true;
          result->isFunction = true;
          result->name = tok->s;
          result->parameters.push_back(move(f1));
          result->parameters.push_back(move(f2));
          f1 = move(result);
        }
        else
          break;
      }
      return f1;
    }
  };

  bool stackNotCommaClosePar(parsingContext& context, vector<unique_ptr<Expr>>& list);
  unique_ptr<Expr> parseapp(parsingContext& context);

  class ParserApp: public Parser {
  public:
    virtual unique_ptr<Expr> parse(parsingContext& context){
      return parseapp(context);
    }
  };

  unique_ptr<Expr> parsecomp(parsingContext& context);

  class ParserComp: public Parser {
  public:
    virtual unique_ptr<Expr> parse(parsingContext& context){
      return parsecomp(context);
    }
  };

  Parser* parseApp = new ParserApp;
  Parser* parseComp = new ParserComp;
  Parser* parseTimes = new ParserBinOps("*/", parseComp);
  Parser* parsePlus = new ParserBinOps("+-", parseTimes);
  Parser* parseUMinus = new ParserUnaryOps("-", parsePlus);
  Parser* parseMod = new ParserBinOps("%", parseUMinus);

  unique_ptr<Expr> parse(parsingContext& context){
    return parseMod->parse(context);
  }

  unique_ptr<Expr> parse(string s){
    return parse(*(new parsingContext(s)));
  }

  enum GetVarOption { NewVar, ExistingVar, None};

  unique_ptr<string> getvar(parsingContext& context, GetVarOption option){
    if (!HasToken(context))
      return 0;
    unique_ptr<token> tokVar = NextToken(context);
    if (tokVar->type != Ident)
      return 0;
    context.nextPos = tokVar->nextPos;
    string result = tokVar->s;
    switch(option){
    case NewVar:
      cout << result << " " << *context.nextid(result) << endl;
      //      result = result + "_" + *context.nextid(result);
      break;
    case ExistingVar:
      //      cout << result << " " << *context.id(result) << endl;
      //      result = result + "_" + *context.id(result);
      break;
    case None:
      break;
    }
    return unique_ptr<string>(new string(result));
  }

  unique_ptr<Expr> parseapp(parsingContext& context){
    //result;
    if (HasToken(context, "(")){
      unique_ptr<Expr> recPar;
      unique_ptr<Func> result(new Func);
      recPar = parse(context);
      if (!HasToken(context, ")"))
        faileval;
      result->parameters.push_back(move(recPar));
      result->isFunction = true;
      return move(result);
    }
    unique_ptr<token> tok = NextToken(context);
    switch(tok->type){
    case End:
      return 0;
      break;
    case Ident:
      {
	if (isint(tok->s)){
	  int res = 0;
	  char* nameChars = (char*)(tok->s.data());
	  sscanf(nameChars, "%i", &res);
	  unique_ptr<ValueExpr> result(new ValueExpr(new ValueAny<int>(res)));
	  context.nextPos = tok->nextPos;
	  return move(result);
	}
	if (isdouble(tok->s)){
	  float res = 0;
	  char* nameChars = (char*)(tok->s.data());
	  sscanf(nameChars, "%g", &res);
	  unique_ptr<ValueExpr> result(new ValueExpr(new ValueAny<double>(res)));
	  context.nextPos = tok->nextPos;
	  return move(result);
	}
        unique_ptr<Func> result(new Func);
        result->name = tok->s;//*context.idornot(tok->s);
        context.nextPos = tok->nextPos;
        if (!HasToken(context))
          return move(result);
        if (HasToken(context, "(")){
          result->isFunction = true;
          //int innerNextPos = nextPos;
          while(stackNotCommaClosePar(context, result->parameters));
          //    nextPos = innerNextPos;
        }
        return move(result);
      }
      break;
    case StringCst:
      context.nextPos = tok->nextPos;
      return unique_ptr<Expr>(new StringExpr(tok->s));
    case Op:
      failparse;
    case Keyword:
      failparse;
    case Unknown:
      cout << endl << "Unknown token at pos " << context.nextPos << context.get() << endl;
      failparse;
    }
    return 0;
  }

  unique_ptr<Expr> parsecomp(parsingContext& context){
    unique_ptr<Expr> recPar = parseapp(context);
    if (HasToken(context, ".")){
      unique_ptr<Comp> result(new Comp);
      unique_ptr<string> f = getvar(context, None);
      if (f == 0)
        failparse;
      if (!HasToken(context, "("))
        failparse;
      result->v = move(recPar);
      result->member = *f;
      while(stackNotCommaClosePar(context, result->parameters));
      return move(result);
    }
    return recPar;
  }


  bool stackNotCommaClosePar(parsingContext& context, vector<unique_ptr<Expr>>& list){
    if (HasToken(context, ")"))
      return false;
    list.push_back(parse(context));
    if (HasToken(context, ")"))
      return false;
    else if (HasToken(context, ","))
      return true;
    return false;
  }

  unique_ptr<Block> parseCommands(parsingContext& context);

  unique_ptr<Expr> parseCommand(parsingContext& context){
    if (!HasToken(context))
      return unique_ptr<Expr>(new Block);
    if (HasToken(context, ";"))
      return unique_ptr<Expr>(new Block);
    here;
    if (HasToken(context, "var")){
      unique_ptr<Definition> def(new Definition);
      unique_ptr<string> varName = getvar(context, NewVar);
      if (varName == 0)
        failparse;
      if (HasToken(context, "(")){
        def->isFunction = true;
        if (!HasToken(context, ")"))
          while(true){
            unique_ptr<string> paramName = getvar(context, NewVar);
            if (paramName == 0)
              failparse;
            def->parameters.push_back(*paramName);
            if (!HasToken(context, ",")){
              if (HasToken(context, ")"))
                break;
              failparse;
            }
          }
      }
      if (!HasToken(context, "="))
        failparse;
      if (!HasToken(context))
        failparse;
      unique_ptr<Expr> e = parseCommand(context);
      def->name = *varName;
      def->value = move(e);
      return move(def);
    }
    else if (HasToken(context, "set")){
      unique_ptr<string> varName = getvar(context, ExistingVar);
      if (varName == 0)
        failparse;
      if (!HasToken(context, "="))
        failparse;
      if (!HasToken(context))
        failparse;
      unique_ptr<Expr> e = parse(context);
      if (!HasToken(context, ";"))
        failparse;
      unique_ptr<Assign> def (new Assign);
      def->name = *varName;
      def->value = move(e);
      return move(def);
    }
    else if (HasToken(context, "{")){
      unique_ptr<Expr> c = parseCommands(context);
      if (!HasToken(context, "}"))
        failparse;
      return c;
    }
    else if (HasToken(context, "for")){
      if (!HasToken(context, "("))
        failparse;
      unique_ptr<string> varName = getvar(context, NewVar);
      if (varName == 0)
        failparse;
      if (!HasToken(context, "in"))
        failparse;
      if (!HasToken(context))
        failparse;
      unique_ptr<Expr> e = parse(context);
      if (!HasToken(context, ")"))
        failparse;
      unique_ptr<Expr> c = parseCommand(context);
      unique_ptr<For> fc (new For);
      fc->variable = *varName;
      fc->seq=move(e);
      fc->command=move(c);
      return move(fc);
    }
    else if (HasToken(context, "while")){
      if (!HasToken(context, "("))
        failparse;
      if (!HasToken(context))
        failparse;
      unique_ptr<Expr> cond = parse(context);
      if (!HasToken(context, ")"))
        failparse;
      unique_ptr<Expr> c = parseCommand(context);
      unique_ptr<While> fc (new While);
      fc->cond = move(cond);
      fc->command = move(c);
      return move(fc);
    }
    else if (HasToken(context, "if")){
      if (!HasToken(context, "("))
        failparse;
      if (!HasToken(context))
        failparse;
      unique_ptr<Expr> cond = parse(context);
      if (!HasToken(context, ")"))
        failparse;
      unique_ptr<Expr> ct = parseCommand(context);
      if (!HasToken(context, "else"))
        failparse;
      unique_ptr<Expr> ce = parseCommand(context);
      unique_ptr<Ifte> fc (new Ifte);
      fc->cond = move(cond);
      fc->commandt = move(ct);
      fc->commande = move(ce);
      return move(fc);
    }
    unique_ptr<Expr> e = parse(context);
    if (!HasToken(context, ";"))
      failparse;
    // Call* call = new Call;
    // call->f = e;
    return e;
  }

  unique_ptr<Expr> parseCommand(string s){
    return parseCommand(*(new parsingContext(s)));
  }

  unique_ptr<Block> parseCommands(parsingContext& context){
    vector<unique_ptr<Expr>> commands;
    parsingContext inner (context);
    while(HasToken(inner)){
      here;
      context.nextPos = inner.nextPos;
      if(HasToken(inner, "}"))
        break;
      here;
      commands.push_back(parseCommand(inner));
    }
    here;
    unique_ptr<Block> block (new Block);
    block->commands = move(commands);
    return block;
  }

  unique_ptr<Block> parseCommands(string s){
    return parseCommands(*(new parsingContext(s)));
  }

  ValueAny<double>* doubleValue(Value* p){
      ValueAny<int>* pi = dynamic_cast<ValueAny<int>*>(p);
      if (pi != 0)
        return new ValueAny<double>(pi->value, pi->s);
      return dynamic_cast<ValueAny<double>*>(p);
  }

  ValueAny<int>* intValue(Value* p){
      ValueAny<double>* pd = dynamic_cast<ValueAny<double>*>(p);
      if (pd != 0)
        return new ValueAny<int>(pd->value);
      return dynamic_cast<ValueAny<int>*>(p);
  }

  class FPlus: public FValue {
  public:
    Value* eval(vector<Value*>& parameters){
      if (parameters.size() != 2)
        faileval
      Value* p1 = parameters[0];
      Value* p2 = parameters[1];
      ValueAny<int>* p1i = dynamic_cast<ValueAny<int>*>(p1);
      ValueAny<string>* p1s = dynamic_cast<ValueAny<string>*>(p1);
      ValueAny<int>* p2i = dynamic_cast<ValueAny<int>*>(p2);
      ValueAny<string>* p2s = dynamic_cast<ValueAny<string>*>(p2);
      if (p1s != 0 && p2s != 0)
        return newstring(p1s->value + p2s->value);
      else if (p1i != 0 && p2i != 0)
        return new ValueAny<int>(p1i->value + p2i->value, p1i->s + p2i->s);
      ValueAny<double>* p1d = doubleValue(p1);
      if (p1d == 0)
        faileval;
      ValueAny<double>* p2d = doubleValue(p2);
      if (p2d == 0)
        faileval;
      return new ValueAny<double>(p1d->value + p2d->value, "double value");


      // if (!(p1->isNum && p2->isNum))
      //        faileval;
      // if (p1->kind == Int && p2->kind == Int)
      //        return new Value(p1->intValue + p2->intValue);
      // return new Value(p1->doubleValue + p2->doubleValue);

      // if (p1->kind == p2->kind && p1->kind == String)
      //        new Value(p1->stringValue + p2->stringValue);
      // else if (!(p1->isNum && p2->isNum))
      //        faileval;
      // if (p1->kind == Int && p2->kind == Int)
      //        return new Value(p1->intValue + p2->intValue);
      // return new Value(p1->doubleValue + p2->doubleValue);
    }
    FPlus(){
      name = "+";
    }
  } fplus;

  class FMinus: public FValue {
  public:
    Value* eval(vector<Value*>& parameters){
      if (parameters.size() == 1)
        {
          Value* p1 = parameters[0];
          ValueAny<int>* p1i = dynamic_cast<ValueAny<int>*>(p1);
          if (p1i != 0)
            return new ValueAny<int>(-p1i->value);
          ValueAny<double>* p1d = dynamic_cast<ValueAny<double>*>(p1);
          if (p1d != 0)
            return new ValueAny<double>(-p1d->value);
          faileval;
        }
      else if (parameters.size() != 2)
        faileval;
      Value* p1 = parameters[0];
      Value* p2 = parameters[1];
      ValueAny<int>* p1i = dynamic_cast<ValueAny<int>*>(p1);
      ValueAny<int>* p2i = dynamic_cast<ValueAny<int>*>(p2);
      if (p1i != 0 && p2i != 0)
        return new ValueAny<int>(p1i->value - p2i->value);
      ValueAny<double>* p1d = doubleValue(p1);
      if (p1d == 0)
        faileval;
      ValueAny<double>* p2d = doubleValue(p2);
      if (p2d == 0)
        faileval;
      return new ValueAny<double>(p1d->value - p2d->value);
    }
    FMinus(){
      name = "-";
    }
  } fminus;

  class FMod: public FValue {
  public:
    Value* eval(vector<Value*>& parameters){
      check_parameters(2);
      getint(p1);
      getint(p2);
      return new ValueAny<int>(p1 % p2);
    }
    FMod(){
      name = "%";
    }
  } fmod;

  class FNot: public FValue {
  public:
    Value* eval(vector<Value*>& parameters){
      check_parameters(1);
      getint(p1);
      return new ValueAny<int>(p1 == 0 ? 1 : 0);
    }
    FNot(){
      name = "not";
    }
  } fnot;

#define binop(classname,fname,sname,op) class classname: public FValue { \
public: \
  Value* eval(vector<Value*>& parameters){ \
    if (parameters.size() != 2) \
      faileval; \
    Value* p1 = parameters[0]; \
  Value* p2 = parameters[1]; \
  ValueAny<int>* p1i = dynamic_cast<ValueAny<int>*>(p1); \
  ValueAny<int>* p2i = dynamic_cast<ValueAny<int>*>(p2); \
  if (p1i != 0 && p2i != 0) \
    return new ValueAny<int>(p1i->value op p2i->value); \
  ValueAny<double>* p1d = doubleValue(p1); \
  if (p1d == 0) \
    faileval; \
  ValueAny<double>* p2d = doubleValue(p2); \
  if (p2d == 0) \
    faileval; \
  return new ValueAny<double>(p1d->value op p2d->value); \
  } \
  classname(){ \
    name = sname; \
  } \
  } fname

  binop(FTimes,ftimes,"*",*);
  binop(FDiv,fdiv,"/",/);

  class Fstring: public FValue {
  public:
    Value* eval(vector<Value*>& parameters){
      if (parameters.size() != 1)
        faileval;
      string s = parameters[0]->tostring();
      return newstring(s);
    }
    Fstring(){
      name = "string";
    }
  } fstring;

  class FSeq: public FValue {
  public:
    Value* eval(vector<Value*>& parameters){
      int start, step, stop;
      if (parameters.size() == 2){
        check_parameters(2);
        getint(p0);
        getint(p1);
        start = p0;
        stop = p1;
        step = 1;
      } else{
        check_parameters(3);
        getint(p0);
        getint(p1);
        getint(p2);
        start = p0;
        stop = p2;
        step = p1;
      }
      vector<Value*>* v = new vector<Value*>;
      for (int i = start; i <= stop; i += step)
        v->push_back(new ValueAny<int>(i));
      return new ValueAny<vector<Value*>*>(v, "vector");
    }
    FSeq(){
      name = "seq";
    }
  } fseq;

  class FList_Files: public FValue {
  public:
    Value* eval(vector<Value*>& parameters){
      //int start, step, stop;
      //      if (parameters.size() == 2){
      check_parameters(1);
      getany(string,dirname);
      boost::filesystem::path directory = boost::filesystem::path(dirname);
      vector<string> files;
      if( exists( directory ) )
        {
          boost::filesystem::directory_iterator end ;
          for( boost::filesystem::directory_iterator iter(directory) ; iter != end ; ++iter )
            if ( boost::filesystem::is_directory( *iter ) )
              {
                //cout << iter->native_directory_string() << " (directory)\n" ;
                //if( recurse_into_subdirs ) show_files(*iter) ;
              }
            else{
              files.push_back(iter->path().string());
            }
        }
      sort(files.begin(), files.end());
      vector<Value*>* v = new vector<Value*>;
      for(unsigned int i = 0; i < files.size(); i++){
        //      cout << files[i] << " (file)\n" ;
        v->push_back(newstring(files[i]));
      }
      return new ValueAny<vector<Value*>*>(v, "vector");
    }
    FList_Files(){
      name = "list_files";
    }
  } flist_files;


  class Fget: public member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      check_parameters(2);
      getany(vector<Value*>*, v);
      getint(p);
      return (*v)[p];
    }
    Fget(){
      name = "get";
    }
  } fget;

  class Fset: public member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      check_parameters(2);
      getany(vector<Value*>*, v);
      getint(p);
      (*v)[p] = parameters[2];
      return voidunit();
    }
    Fset(){
      name = "set";
    }
  } fset;

  class Fpush: public member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      check_parameters(2);
      getany(vector<Value*>*, v);
      v->push_back(parameters[1]);
      return voidunit();
    }
    Fpush(){
      name = "push";
    }
  } fpush;


  class Fstringlength: public member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      check_parameters(1);
      getany(string, v);
      return new ValueAny<int>(v.size());
    }
    Fstringlength(){
      name = "length";
    }
  } fstringlength;


  class Fvectorlength: public member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      check_parameters(1);
      getany(vector<Value*>*, v);
      return new ValueAny<int>(v->size());
    }
    Fvectorlength(){
      name = "length";
    }
  } fvectorlength;


  class Fstringsub: public member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      check_parameters(3);
      getany(string, v);
      getint(p);
      getint(l);
      return newstring(v.substr(p, l));
    }
    Fstringsub(){
      name = "sub";
    }
  } fstringsub;


  ValueAny<string>* newstring(string s){
    ValueAny<string>* v = new ValueAny<string>(s, s);
    v->add(&fstringlength)
      ->add(&fstringsub);
    return v;
  }

  ValueAny<vector<Value*>*>* newvector(vector<Value*>* point){
    ValueAny<vector<Value*>*>* v = new ValueAny<vector<Value*>*>(point, "array");
    v->add(&fget)
      ->add(&fset)
      ->add(&fpush)
      ->add(&fvectorlength);
    return v;
  }

  class Fnewvector: public FValue {
  private:
    vector<Value*> values;
  public:
    Value* eval(vector<Value*>& parameters){
      auto result = new vector<Value*>;
      for (unsigned int i = 0; i < parameters.size(); i++)
        result->push_back(parameters[i]);
      return newvector(result);
    }
    Fnewvector(){
      name = "newvector";
    }
  } fnewvector;

  Env* newenv(){
    Env* env = new Env();
    env->functions.push_back(&fseq);
    env->functions.push_back(&fplus);
    env->functions.push_back(&fminus);
    env->functions.push_back(&ftimes);
    env->functions.push_back(&fdiv);
    env->functions.push_back(&fnot);
    env->functions.push_back(&fmod);
    env->functions.push_back(&fnewvector);
    env->functions.push_back(&fstring);
    env->functions.push_back(&flist_files);
    return env;
  }

  string read(string filename){
    ifstream fin(filename.data());
    char result[10000];
    char ch;
    int pos = 0;
    for(; pos < 10000; pos++)
      if (fin.get(ch))
	result[pos] = ch;
      else
	break;
    fin.close();
    string s = result;
    s.resize(pos);
    return s;
  }

  unique_ptr<Expr> load(int argc, char** argv){
    unique_ptr<Block> scenes(new Block);
    string readopt = "-s";
    string evalopt = "-e";
    for (int i = 0; i < (argc - 1) / 2; i++){
      here;
      string s;
      if (argv[2 * i + 1] == readopt)
	s = read(argv[2 * i + 2]);
      else if (argv[2 * i + 1] == evalopt)
	s = argv[2 * i + 2];
      else{
	cout << "-s <file> | -e <expr>" << endl;
	throw new EvalException();
      }
      unique_ptr<Block> c = parseCommands(s);
      for(unsigned int j = 0; j < c->commands.size(); j++)
	scenes->commands.push_back(c->commands[j]->copy());
    }
    return move(scenes);
  }
}
