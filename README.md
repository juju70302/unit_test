# unit_test

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
