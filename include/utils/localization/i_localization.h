#ifndef I_LOCALIZATION_H
#define I_LOCALIZATION_H

class ILocalization
{

public:
    virtual ~ILocalization() = default;
    virtual void listener_update_ui() = 0;
};
#endif // I_LOCALIZATION_H