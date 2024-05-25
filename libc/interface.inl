typedef struct {
  char const *data;
  size_t size;
} string_view;
typedef struct {
  char *data;
  size_t size;
  size_t capacity;
} string;
string_view _getenv(char const *ptr);
void *_malloc(size_t m);
void _free(void *ptr);
void *_realloc(void *ptr, size_t size);
void _memset(void *ptr, int value, size_t size);
int _memcmp(void const *a, void const *b, size_t size);
char _char_at(char const *ptr, size_t idx);
string string_make(size_t capacity);
void string_dispose(string *s);
void string_reserve(string *s, size_t new_capacity);
void string_add_chr(string *s, char const *ptr);
void string_add_str(string *s, char const *ptr, size_t size);

string_view string_subview(string *s, size_t beg, size_t end);
string string_from_file(char const *name);
bool string_to_file(string *s, char const *name);
string string_from_file_str(string *v);
bool string_to_file_str(string *s, string *v);
string string_from_file_strview(string_view v);
bool string_to_file_strview(string *s, string_view v);
void string_set(string *s, size_t idx, char c);
