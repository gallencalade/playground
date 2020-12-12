#ifndef CURRIED_H
#define CURRIED_H

#include <tuple>

template <typename Function, typename... CapturedArgs>
class Curried {
  using ArgsTuple = std::tuple<std::decay_t<CapturedArgs>...>;

 public:
  Curried(Function func, CapturedArgs ...args)
    : function_(func)
    , args_(std::move(args)...) {  }

  Curried(Function func, std::tuple<CapturedArgs...> args)
    : function_(func)
    , args_(std::move(args)) {  }

  template <typename... NewArgs>
  auto operator()(NewArgs ...args) const {
    auto new_args = CopyArgs(std::forward<NewArgs>(args)...);
    auto all_args = std::tuple_cat(args_, std::move(new_args));

    if constexpr (std::is_invocable_v<Function, CapturedArgs..., NewArgs...>()) {
      return std::apply(function_, all_args);
    } else {
      return Curried<Function, CapturedArgs..., NewArgs...>(function_, all_args);
    }
  }

 private:
  template <typename... CopiedArgs>
  static auto CopyArgs(CopiedArgs&& ...args) {
    return std::tuple<std::decay_t<CopiedArgs>...>(std::forward<CopiedArgs>(args)...);
  }

  Function function_;
  std::tuple<CapturedArgs...> args_;
};

#endif  // CURRIED_H
