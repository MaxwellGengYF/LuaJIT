local ffi = require("ffi")
libc = ffi.load("libc")
ffi.cdef [[
  typedef struct {
    char const *data;
    size_t size;
  } string_view;
  typedef struct {
    char  *data;
    size_t size;
    size_t capacity;
  } string;
  string_view _getenv(char const *ptr);
  void *_malloc(size_t m);
  void _free(void *ptr);
  void *_ralloc(void *ptr, size_t size);
  void _memset(void* ptr, int value, size_t size);
  int _memcmp(void const* a, void const* b, size_t size);
  char _char_at(char const* ptr, size_t idx);
  
  string string_make(size_t capacity);
  void string_dispose(string *s);
  void string_add_chr(string* s, char const* ptr);
  void string_add_str(string* s, char const* ptr, size_t size);
  
  string_view string_subview(string *s, size_t beg, size_t end);
  string string_from_file(char const* name);
  bool string_to_file(string *s, char const* name);
  string string_from_file_str(string* v);
  bool string_to_file_str(string *s, string* v);
  string string_from_file_strview(string_view v);
  bool string_to_file_strview(string *s, string_view v);
  void string_set(string* s, size_t idx, char c);
]]
string_view = ffi.metatype("string_view", {
    __len = function(s)
        return s.size
    end,
    __index = {
        to_string = function(s)
            return ffi.string(s.data, s.size)
        end,
        at = function(s, idx)
            return libc._char_at(s.data, idx)
        end,
        empty = function(s)
            return s.size == 0
        end
    }
})

string = ffi.metatype("string", {
    __len = function(s)
        return s.size
    end,
    __index = {
        to_string = function(s)
            return ffi.string(s.data, s.size)
        end,
        at = function(s, idx)
            return libc._char_at(s.data, idx)
        end,
        empty = function(s)
            return s.size == 0
        end,
        dispose = function(s)
            libc.string_dispose(s)
        end,
        add = function(s, v)
            if type(v) == "string" then
                return libc.string_add_chr(s, v)
            else
                return libc.string_add_str(s, v.data, v.size)
            end
        end,
        to_file = function(s, name)
            if type(name) == "string" then
                return libc.string_to_file(s, name)
            end
            if name.capacity ~= nil then
                return libc.string_to_file_str(s, name)
            end
            return libc.string_to_file_strview(s, name)
        end
    }
})

function make_string(arg)
    if arg == nil then
        arg = 0
        return libc.string_make(arg)
    end
    if type(arg) == "number" then
        return libc.string_make(arg)
    end
    if type(arg) == "string" then
        local s = libc.string_make(#arg)
        s:add(arg)
        return s
    end
end

function read_file(arg)
    if type(arg) == "string" then
        return libc.string_from_file(arg)
    end
    if arg.capacity ~= nil then
        return libc.string_from_file_str(arg)
    end
    return libc.string_from_file_strview(arg)
end
