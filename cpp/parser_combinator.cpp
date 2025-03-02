#include <string_view>
#include <type_traits>
#include <functional>

namespace parser{
using ParseError = std::string_view;
enum class Tag { Ok, Err };
template<typename Output>
struct ParseResult {
    Tag tag;
    union {
        struct {
            std::string_view remain;
            Output value;
        } ok;
        ParseError err;
    };
    bool is_ok() const { return tag == Tag::Ok; }
    static ParseResult Ok(Output value, std::string_view remain){
        return ParseResult {
            .tag = Tag::Ok,
            .ok = {
                .remain = remain,
                .value = value
            }
        };
    }
    static ParseResult Err(ParseError err){
        return ParseResult {
            .tag = Tag::Err,
            .err = err
        };
    }
};

template<typename P, typename Output>
concept Parser = requires(P p, std::string_view input) {
    { p(input) } -> std::same_as<ParseResult<Output>>;
};
//std::same_as<std::function<ParseResult<Output>(std::string_view)>

// template<typename P> requires (Parser<P, std::invoke_result<P, std::string_view>>)
template<typename Output, typename P> requires (Parser<P, Output> && std::same_as<ParseResult<Output>, std::invoke_result_t<P, std::string_view>>)
auto parse(P parser, std::string_view input) -> ParseResult<Output> {
    return parser(input);
}

/* combinator */
template<typename A, typename P, typename MapFn, typename B=std::invoke_result_t<MapFn, A>>
requires (Parser<P, A> && std::invocable<MapFn, A> && std::same_as<std::invoke_result_t<MapFn, A>, B>)
auto map(P p, MapFn map_fn)  {
    return [=](std::string_view input) {
        auto result = p(input);
        if (result.tag == Tag::Ok) {
            return ParseResult<B>::Ok(map_fn(result.ok.value), result.ok.remain);
        } else {
            return ParseResult<B>::Err(result.err);
        }
    };
}


/* Operand */
auto need_char(char c) {
    return [=](std::string_view input) {
        if (input.empty()) {
            return ParseResult<char>::Err("unexpected end of input");
        } else if (input[0] == c) {
            return ParseResult<char>::Ok(c, input.substr(1));
        } else {
            return ParseResult<char>::Err( "unexpected character");
        }
    };
}

auto predicate(std::function<bool(char)> pred) {
    return [=](std::string_view input) {
        if (input.empty()) {
            return ParseResult<char>::Err("unexpected end of input");
        } else if (pred(input[0])) {
            return ParseResult<char>::Ok(input[0], input.substr(1));
        } else {
            return ParseResult<char>::Err("unexpected character");
        }
    };
}

}
int main(int argc, char *argv[]) {
    auto parser = parser::map<char>(parser::need_char('a'), [](char c) -> int { return c - 'a'; });
    auto result = parser::parse<int>(parser, std::string_view("abc"));
    if (result.tag == parser::Tag::Ok) {
        printf("remain: %s, value: %d\n", result.ok.remain.data(), result.ok.value);
    } else {
        printf("error: %s\n", result.err.data());
    }
    return 0;
}
