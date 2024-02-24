## Concerns
The following conerns were identified
1. Easy of use prevails of precise formatting
2. A macro-free logger front-end is desirable, as macros:
   * may trigger/expose unintended side-effects
   * may hide build errors until a particular log-level is enabled
   * discouraged by ISOCPP
3. Allocation in the  logger may induce bad alloc errors
4. Unconstrained log message size accompanied with C++ operators may cause unintentionally huge data dumps
5. Troubleshooting race conditions requires sync-free logging
6. Log level limits should be available for build-time and run-time
7. Sink selection should be available for build-time and run-time
8. Logger tuning should be flexible enough to focus on a particular problem and void unrelated records
9. Logger front-end should introduce minimal possible penalties and overhead
10. Changing log level should not impact any possible side-effects of the user’s logging statements
11. Logger’s formatter should not interfere with application’s formatters
12. Access to the logger is needed from different contexts, such as static methods, free functions, lambdas.
13. A shared instance of a logger front-end would require some synchronization technique
14. In some use cases the log messages should be attributed with some common information, such as date, tag, source location, thread-id, process id.
15. Some log sinks automatically attribute the messages, some others don’t
16. In some cases log messages should be routed to more than one sink.
    * This multi-sink routing may need some logic based on the message attributes
    * Such sinks may have different approach or capabilities on message attributing
17. Logger from-end is the only source of source location information
18. An application should not require any run-time configuration file to use logger
    * run-time configuration file is an option, not a prerequisite
19. Some libraries needs logging, however, where to log and whether tog should be decided by application
20. Shared logger configuration is better for consistency, ad-hoc overrides – for flexibility
21. Some standard to string conversions are available only for `operator<<(std::ostream,...) .`
22. Different teams prefer different syntax of the log statements – function calls, stream left shift operators, format string
