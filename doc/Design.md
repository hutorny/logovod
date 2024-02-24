## Design Philosophy
Focus on a simple ready-to-use solution and provide easy-to-use means for making custom solutions as complex as needed 

## Design Decisions
1. Logger configuration responsibilities are assigned to concept of log Category.
2. Log Category defines logger properties and controls it behavior
3. Logger is a template class parametrized by a log category
4. Log emitters are inner classes of the logger template, additionally parametrized by a log priority level
5. Log sink is a set of weakly related concepts – prologer, writer, epiloger.
    * prologer, writer, epiloger are free functions. 
    * Any state needed for their operations maintained externally, for example in variables with static storage class
6. A category may redefine sink concepts as a functor object, if needed
7. An emitter:
    * buffers data writes, issued by the user,
    * collects message attributes, 
    * calls prologer, epiloger and 
    * flushes the message to the writer
8. An emitter is considered to be a disposable, one-time use object. 
    * Design, however, should not disallow other use cases, such as emitter reuse, or deferred use.
9. Emitter  supports three style of use: invoke , left shift and format string
10. Emitter deploys std::ostream for converting value to text
11. Emitter uses internally allocated stream buffer of capacity, defined in the category
    * If category defines unlimited capacity, basic_stringbuf is used
