/******************************************************************************************
*
*   An automated unit-testing frame for single function.
*   Must be compiled with C++ 11 or higher version.
*
*   2020-08-03  Updating annotation.
*   2020-11-12  Updating class function "CaseMessage"
*
*   Design by Cheng-Yung-Sheng
*
******************************************************************************************/
#include <vector>
#include <string>
#include <tuple>
#include <sstream>

#ifndef UNITTEST_H_
#define UNITTEST_H_

//
//  TestFunction:
//      Unit test for simple function.User can inherit from class "TestFunction",
//      re-write 2 pure member functions and input test cases to test user's function.
//
/***********************************Sample usage:*****************************************
int sum(int a, int b) { return a + b; }

void testSum()
{
#define testType h1
    typedef tuple<int, int> para_tuple;
    typedef int return_type;

    class testA : public TestFunction<para_tuple, return_type>
    {
    private:
        void RunFunction(const para_tuple &para, return_type &returnValue) override
        {
            returnValue = sum(std::get<0>(para), std::get<1>(para));
        };
        std::string CaseMessage(const para_tuple &para, const vector<return_type> &e_v, const return_type &r_v) override
        {
            std::stringstream ss;
            ss.str("");
            ss.clear();

            ss << "1st parameter = \"" << std::get<0>(para) << "\"" << std::endl
               << "2nd parameter = \"" << std::get<1>(para) << "\"" << std::endl
               << "expected_value=\"" << VectorToString<int>(e_v) << "\"" << std::endl
               << "return_value=\"" << r_v << "\"" << endl;
            return ss.str();
        };
    } test;

    test.AddTestCase(2, make_tuple(1, 1));
    test.AddTestCase(6, make_tuple(2, 4));

    test.Run();
    std::cout << test.SimpleResultMessage("sum()") << std::endl;

#undef testType
}
******************************************************************************************/

//  para_tuple:The tuple with tested function's parameters.User must define while inheriting.
//  return_type:The type of return value.User must define while inheriting.
template <typename para_tuple, typename return_type>
class TestFunction
{
public:
    //  Virtual function for running tested function once.User must re-write before correctly using it.
    //  The first parameter is tuple of tested function's parameter(or para_tuple mentioned above),
    //      and second parameter is to record result(return value) of tested function.
    //  Ideal process is that "RunFunction" runs tested function once and store result in r_v.
    virtual void RunFunction(const para_tuple &para, return_type &r_v) = 0;

    //  Virtual unction for producing case's message.Users are recommanded to re-write before correctly using it.
    //  If user doesn't re-write,parameter would be unable to display.
    //  It's for costumized display case information with consiceness.
    //  1st,2nd and 3rd parameter is tuple of tested function's parameters,vector of expected return values
    //      and actually return value, respectively.
    //virtual std::string CaseMessage(const para_tuple &para, const std::vector<return_type> &e_v, const return_type &r_v) = 0;
    virtual std::string CaseMessage(const para_tuple &para, const std::vector<return_type> &e_v, const return_type &r_v)
    {
        std::stringstream ss;
        ss.str("");
        ss.clear();

        ss << "expected value=";
        if (e_v.size() <= 1)
        {
            ss << "\"" << e_v.at(0) << "\"" << std::endl;
        }
        else
        {
            for (int i = 0; i < e_v.size(); i++)
                ss << std::endl
                   << e_v.size();
            ss << std::endl;
        }

        ss << "return value=\"" << r_v << "\"" << std::endl;

        return ss.str();
    };

    //  Function for distinguishing if 2 returned value equal.
    //  User can re-write or using default compare operation.
    //  2 parameter with type of return_type is compared values.
    virtual int IsEqual(const return_type &a1, const return_type &a2) { return a1 == a2; };

    //  A convinient function for users to transfer vector's values into string.
    //  "eleType" is type of vector's element.The "eleType" must be able to be recogonized by stringstream.
    template <typename eleType>
    std::string VectorToString(const std::vector<eleType> &vec)
    {
        std::stringstream ss;
        ss.str("");
        ss.clear();

        for (int i = 0; i < vec.size(); i++)
        {
            if (i != 0)
                ss << ",";
            ss << vec.at(i);
        }

        return ss.str();
    };

    //  A convinient function for users to transfer vector's values into string.
    //  "eleType" is type of vector's element.
    //  1st parameter is function pointer which tranfering "eleType" into string.
    //  It return string with all vector's element(2nd parameter).
    template <typename eleType>
    std::string VectorToString(std::string (*toString)(const eleType &), const std::vector<eleType> &vec)
    {
        std::stringstream ss;
        ss.str("");
        ss.clear();

        for (int i = 0; i < vec.size(); i++)
        {
            if (i != 0)
                ss << " , ";
            ss << toString(vec.at(i));
        }

        return ss.str();
    };

    // Constructor of class.It clear class's member "test_case_" and "test_result_".
    TestFunction()
    {
        test_case_.clear();
        test_result_.clear();
    };

    //  Adding test cases into class member "test_case_".
    //  1st parameter "e_v" is the vector of expected return values of tested function.
    //  2nd parameter "para" is the tuple of parameter of tested function in this test case.
    //  This function only add case but do not run it immediately.
    void AddTestCase(const std::vector<return_type> &e_v, const para_tuple &para)
    {
        test_case_.push_back(std::pair<para_tuple, std::vector<return_type>>(para, e_v));
    };

    //  Adding test cases into class member "test_case_".
    //  1st parameter "e_v" is expected return values of tested function.
    //  2nd parameter "para" is the tuple of parameter of tested function in this test case.
    //  Because test cases is stored with mutiple expected return values,
    //      this function transfer single expected value into mutiple one(by vector) for user convenience.
    //  This function only add case but do not run it immediately.
    void AddTestCase(const return_type &e_v, const para_tuple &para)
    {
        AddTestCase(std::vector<return_type>{e_v}, para);
    }

    //  Running all test cases.
    //  Results will be record into class member "test_result_".
    void Run()
    {
        return_type return_value;
        test_result_.clear();
        test_result_.resize(test_case_.size());

        for (int case_n = 0; case_n < test_case_.size(); case_n++)
        {
            //run nth case...
            RunFunction(test_case_.at(case_n).first, test_result_[case_n].second);

            //check whether result is expected...
            test_result_[case_n].first = AnswerFit::UNFIT;
            std::vector<return_type> *this_expec = &(test_case_.at(case_n).second);
            for (int i = 0; i < this_expec->size(); i++)
            {
                //if (testResult[case_n].second == this_expec->at(i))
                if (IsEqual(test_result_[case_n].second, this_expec->at(i)))
                {
                    test_result_[case_n].first = FIT;
                    break;
                }
            }
        }
    };

    //  Comboming test results and translating them into string to return.
    //  This function using virtual member function "CaseMessage()" defined by user.
    //  User had better call member function "Run()" before call this function.
    std::string TestMessage()
    {
        std::stringstream ss, err;
        ss.str("");
        ss.clear();
        err.str("");
        err.clear();

        int right_n = 0, failure_n = 0;
        for (int i = 0; i < test_result_.size(); i++)
        {
            if (test_result_.at(i).first == FIT)
            {
                right_n++;
            }
            else if (test_result_.at(i).first == AnswerFit::UNFIT)
            {
                failure_n++;
                std::string caseMsg = "", headerStar = "";
                int starNum = 0;
                caseMsg = CaseMessage(test_case_.at(i).first, test_case_.at(i).second, test_result_.at(i).second);

                starNum = 0;
                for (int i = 0; i < caseMsg.length(); i++, starNum++)
                    if (caseMsg[i] == '\n')
                        break;
                starNum = starNum > 50 ? 50 : starNum;
                ;
                for (int j = 0; j < starNum; j++)
                    headerStar += "*";
                err << headerStar << "case " << failure_n << headerStar << std::endl
                    << std::endl
                    << caseMsg << std::endl
                    << headerStar << "*end*" << headerStar << std::endl
                    << std::endl
                    << std::endl;
            }
        }

        ss << "fit cases = \"" << right_n << "\"\tunmatched cases = \"" << failure_n << "\"" << std::endl;
        if (failure_n)
            ss << "unmatched cases >>" << std::endl
               << std::endl
               << err.str();
        return ss.str();
    };

    //  Comboming test results and translating them into string to return.
    //  Different from member function "TestMessage()",
    //      this function only decide whether test cases is all expected.
    //  User had better call member function "Run()" before call this function.
    std::string SimpleResultMessage(const std::string &function_name)
    {
        std::stringstream ss;
        ss.str("");
        ss.clear();

        ss << function_name << " ";
        if (IsAllMatched())
            ss << "is safe...";
        else
            ss << "is dangerous!!";
        return ss.str();
    };

    //  Get number of test cases.
    int CaseNum() { return test_case_.size(); };

    //  Get number of test case in which return value matches (one of) expected value.
    //  User had better call member function "Run()" before call this function.
    int MatchedNum()
    {
        int rN = 0;
        for (int i = 0; i < test_result_.size(); i++)
            if (test_result_.at(i).first == AnswerFit::FIT)
                rN++;
        return rN;
    };

    //  Get number of test case in which return value do not matches (one of) expected value.
    //  User had better call member function "Run()" before call this function.
    int UnmatchedNum() { return test_result_.size() - MatchedNum(); };

    //  Deciding whether all test cases run as expected.
    //  User had better call member function "Run()" before call this function.
    int IsAllMatched() { return (MatchedNum()) ? ((UnmatchedNum()) ? 0 : 1) : 0; };
    int IsSafe() { return IsAllMatched(); };

public:
    //  A enumeration to record whether test return value fit respected value
    enum AnswerFit
    {
        UNFIT = 0,
        FIT = 1
    };

private:
    //  Class member to record test cases.
    //  First member of std::pair is function's parameter.
    //  Second member of std::pair is test cases' expected value.
    std::vector<std::pair<para_tuple, std::vector<return_type>>> test_case_;

    //  Class member to record test result.
    //  First member of std::pair is fit or not(with expected value).
    //  Sencond one is return value of test.
    std::vector<std::pair<AnswerFit, return_type>> test_result_;
};

#endif