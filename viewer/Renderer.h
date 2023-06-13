#pragma once

#include <QObject>
#include <filesystem>

class Renderer : public QObject
{
    Q_OBJECT
public:
    Renderer(std::filesystem::path hipbin = getHipBin());

private:
    static std::filesystem::path getHipBin();
};
