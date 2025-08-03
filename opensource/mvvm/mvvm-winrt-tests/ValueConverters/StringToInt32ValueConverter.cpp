#include "pch.h"
#include "StringToInt32ValueConverter.h"
#if __has_include("StringToInt32ValueConverter.g.cpp")
#include "StringToInt32ValueConverter.g.cpp"
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::UI::Xaml::Interop;
}

using namespace winrt::mvvm::tests::implementation;

winrt::IInspectable StringToInt32ValueConverter::Convert(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, [[maybe_unused]] hstring const& language)
{
    if (!value && !parameter)
        return value;

    auto& passedValue = !value ? parameter : value;
    hstring stringValue = winrt::unbox_value_or(passedValue, {});
    if (stringValue.empty())
        return winrt::box_value(passedValue);

    auto intValue = _wtoi(stringValue.c_str());
    return winrt::box_value(intValue);
}

winrt::IInspectable StringToInt32ValueConverter::ConvertBack([[maybe_unused]] IInspectable const& value, [[maybe_unused]] TypeName const& targetType, [[maybe_unused]] IInspectable const& parameter, [[maybe_unused]] hstring const& language)
{
    return winrt::Windows::Foundation::IInspectable();
}
