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
	diff_result(std::uint64_t begin_, std::uint64_t end_, const std::vector<std::string>& str_, diff_state state_)
		: begin(begin_), end(end_), str(str_), state(state_){};
	diff_result(std::uint64_t begin_, std::uint64_t end_, const std::vector<std::string>& str_, std::uint64_t begin_ch_,
				std::uint64_t end_ch_, const std::vector<std::string>& str_ch_)
		: begin(begin_), end(end_), begin_ch(begin_ch_), end_ch(end_ch_), str(str_), ch_str(str_ch_),
		  state(diff_state::changed){};
	std::uint64_t begin, end, begin_ch, end_ch;
	std::vector<std::string> str, ch_str;
	diff_state state;
};

std::vector<diff_result> diff(const std::string&, const std::string&);
