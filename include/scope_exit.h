#pragma once
#include <concepts>

template <typename Action>
requires (std::is_nothrow_copy_constructible_v<Action> || std::is_nothrow_move_constructible_v<Action>) && std::is_nothrow_invocable_v<Action>
class scope_exit
{
public:
    scope_exit(const Action& action) noexcept : m_action(action) {}
    scope_exit(Action&& action) noexcept : m_action(std::forward<Action&&>(action)) {}
    scope_exit(scope_exit&& rhs) noexcept
        : m_action(std::move(rhs.m_action))
        , m_do{ rhs.m_do }
    {
        rhs.release();
    }
    ~scope_exit() noexcept {
        if (m_do)
            m_action(); 
    }
    void release() noexcept { m_do = false; }
private:
    scope_exit(const scope_exit&) = delete;
    scope_exit& operator=(const scope_exit& ) = delete;
    scope_exit& operator=(      scope_exit&&) = delete;
private:
    Action m_action;
    bool m_do = true;
};

template <typename Action>
requires (std::is_nothrow_copy_constructible_v<Action> || std::is_nothrow_move_constructible_v<Action>) && std::is_nothrow_invocable_v<Action>
auto make_scope_exit(Action&& action) 
noexcept ->decltype(scope_exit< typename std::remove_reference<Action>::type >{ std::forward<Action&&>(action) })
{
    return scope_exit< typename std::remove_reference<Action>::type >{ std::forward<Action&&>(action) };
}
