
#define DUMP_FILE_HEAD          \
"#ifndef __MAPPED_LAYOUT_H__\n" \
"#define __MAPPED_LAYOUT_H__\n" \
"\n"                            \
"#include <map>\n"              \
"#include \"record_layout.h\"\n"\
"\n"                            \
"namespace {\n"                 \
"\n"                            \
"static const std::map<std::string, RecordLayout> kMappedLayout = {\n"

#define DUMP_FILE_TAIL          \
" };\n"                         \
"\n"                            \
"}\t// namespace\n"             \
"\n"                            \
"#endif  // __MAPPED_LAYOUT_H__"
