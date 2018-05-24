#define LOGURU_IMPLEMENTATION 1
#include <stdio.h>
#include <string>
#include <loguru.hpp>

int expensive_check(int x)
{
    sleep(x);
    return 1;
}

int some_expensive_operation()
{
    sleep(2);
    return 0;
}

int main(int argc, char** argv)
{
	loguru::init(argc, argv);
	LOG_SCOPE_FUNCTION(INFO);
	LOG_F(INFO, "Doing some stuff...");
	for (int i=0; i<2; ++i) {
		VLOG_SCOPE_F(1, "Iteration %d", i);
		auto result = some_expensive_operation();
		LOG_IF_F(WARNING, result == 0, "Bad result");
        LOG_IF_F(INFO, result == 1, "Good result");
	}
	
    LOG_F(INFO, "Time to go!");
	LOG_SCOPE_F(INFO, "Will indent all log messages within this scope.");
    LOG_F(INFO, "I'm hungry for some %.3f!", 3.14159);
    LOG_F(2, "Will only show if verbosity is 2 or higher");
    
    // VLOG_F(get_log_level(), "Use vlog for dynamic log level (integer in the range 0-9, inclusive)");
    // LOG_IF_F(ERROR, badness, "Will only show if badness happens");

    std::string filename = "README.md";
    // nullptr_t nullptr fp;
    auto fp = fopen(filename.c_str(), "r");
    
    CHECK_F(fp != NULL, "Failed to open file '%s'", filename.c_str());
	
    int length = 4;
    CHECK_GT_F(length, 0); // Will print the value of `length` on failure.
    
    int a=2, b=2;
    CHECK_EQ_F(a, b, "You can also supply a custom message, like to print something: %d", a + b);

    int x=11;
    // Each function also comes with a version prefixed with D for Debug:
    DCHECK_F(expensive_check(x)); // Only checked #if !NDEBUG
    DLOG_F(INFO, "Only written in debug-builds");

    loguru::set_fatal_handler([](const loguru::Message& message){
        	throw std::runtime_error(std::string(message.prefix) + message.message);
    });

    assert(0==0);

    return 0;
}