typedef enum mem_strategies { FIRST, NEXT, BEST } mem_strategy_t;

int mem_allocate(mem_strategy_t strategy, unsigned int size, unsigned int duration);

int mem_single_time_unit_transpired();

int mem_fragment_count(int frag_size);

void mem_clear();

void mem_init( unsigned int size );

void mem_free();
