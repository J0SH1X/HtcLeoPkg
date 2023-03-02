extern int critical_section_count;

/*static inline __ALWAYS_INLINE void enter_critical_section(void);
static inline __ALWAYS_INLINE void exit_critical_section(void);
static inline __ALWAYS_INLINE bool in_critical_section(void);*/

//static inline void enter_critical_section(void);
//static inline void exit_critical_section(void);
void enter_critical_section(void);
void exit_critical_section(void);
