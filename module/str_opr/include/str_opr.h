#include <compare>
#include <string>
#include <tuple>
#include <vector>

enum class diff_state
{
	deleted,
	added,
	changed
};

class diff_result
{
public:
	diff_result(std::uint64_t begin_, std::uint64_t end_, const std::vector<std::string>& str_,
				std::uint64_t begin_ch_, std::uint64_t end_ch_, const std::vector<std::string>& str_ch_, diff_state state_)
		: orig_begin(begin_), orig_end(end_), new_begin(begin_ch_), new_end(end_ch_), orig_str(str_), new_str(str_ch_),
		  state(state_){};
	std::uint64_t orig_begin, orig_end, new_begin, new_end;
	std::vector<std::string> orig_str, new_str;
	diff_state state;
};

auto operator<=>(const diff_result& a, const diff_result& b);

std::vector<diff_result> diff(const std::string&, const std::string&);
