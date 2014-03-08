#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <QCoreApplication>
#include <QStringList>
#include <string>
#include <set>
#include <sstream>
#include <cereal/types/string.hpp>
#include <cereal/types/set.hpp>
#include <cereal/archives/json.hpp>
using namespace std;

struct Config
{
public:
    Config();

    template <typename Archive>
    void serialize ( Archive& ar )
    {
        ar(
        cereal::make_nvp("thumbWidth",  thumbWidth),
        cereal::make_nvp("rowCount",      rowCount),
        cereal::make_nvp("colCount",       colCount),
        cereal::make_nvp("email",            email),
        cereal::make_nvp("password",      password),
        cereal::make_nvp("session",         session),
        cereal::make_nvp("lastView",        lastView),
        cereal::make_nvp("imageDirs",     imageDir),
        cereal::make_nvp("javDirs",          javDirs),
        cereal::make_nvp("pornDirs",       pornDirs),
        cereal::make_nvp("hentaiDirs",    hentaiDirs)
      );
    }

    int thumbWidth, rowCount, colCount;
    set<string> javDirs, pornDirs, hentaiDirs;
    string email, password, session, imageDir, lastView;

    QStringList javList, pornList, hentaiList;
};

#endif // CONFIG_HPP
