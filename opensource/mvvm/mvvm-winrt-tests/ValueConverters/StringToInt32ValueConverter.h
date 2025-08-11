#pragma once
#include "StringToInt32ValueConverter.g.h"

namespace winrt::mvvm::tests::implementation
{
    struct StringToInt32ValueConverter : StringToInt32ValueConverterT<StringToInt32ValueConverter>
    {
        StringToInt32ValueConverter() = default;

        winrt::Windows::Foundation::IInspectable Convert(winrt::Windows::Foundation::IInspectable const& value, winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, winrt::Windows::Foundation::IInspectable const& parameter, hstring const& language);
        winrt::Windows::Foundation::IInspectable ConvertBack(winrt::Windows::Foundation::IInspectable const& value, winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, winrt::Windows::Foundation::IInspectable const& parameter, hstring const& language);
    };
}

namespace winrt::mvvm::tests::factory_implementation
{
    struct StringToInt32ValueConverter : StringToInt32ValueConverterT<StringToInt32ValueConverter, implementation::StringToInt32ValueConverter>
    {
    };
}
