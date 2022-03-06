#include "chartest.h"

using namespace literals::regex;

const _ct::BasicCharTest ct::isWS{ "\\s"_re };
const _ct::BasicCharTest ct::isNotNL{ "[\n]"_re, false };
const _ct::BasicCharTest ct::isComment{ "[#]"_re };

const _ct::BasicCharTest ct::isDigit{ "\\d"_re };
const _ct::BasicCharTest ct::isID{ "[a-zA-Z0-9_?]"_re };

const _ct::BasicCharTest ct::isCharQutoe{ "[\']"_re };
const _ct::BasicCharTest ct::isStringQuote{ "[\"]"_re };

const _ct::BasicCharTest ct::isOp{ "[\\+\\-\\*\\/\\%=<>.:]"_re };
const _ct::BasicCharTest ct::isPunc{ "[\\{\\[\\(;,)\\]}}]"_re };

const _ct::BasicCharTest ct::isKeyword{ "true|false|if|else|def|return"_re };
