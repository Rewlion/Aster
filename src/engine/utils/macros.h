#pragma once

#define MACRO_CONCAT2(a,b) a ## b
#define MACRO_CONCAT(a,b) MACRO_CONCAT2(a,b)
#define NAME_WITH_LINE(name) MACRO_CONCAT(name,__LINE__)
