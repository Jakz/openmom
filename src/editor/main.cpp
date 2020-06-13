#include <algorithm>
#include <unordered_map>

#include <QtWidgets/QApplication>
#include <QtWidgets/QTableView>
#include <QtWidgets/QHeaderView>

#include <QtCore/QAbstractTableModel>

#include "gfx/LBXRepository.h"
#include "save/YAML.h"

#include "yaml-cpp/node/detail/node_data.h"
std::string YAML::detail::node_data::empty_scalar;

using namespace lbx;

template<typename T> using sprite_cache_t = std::unordered_map<T, QPixmap>;

template<typename T>
class DataTableModel : public QAbstractTableModel
{
protected:
  std::vector<std::pair<QString, T>> _data;

public:
  DataTableModel()
  {
    const auto& data = Data::values<T>();

    auto it = data.begin();

    std::transform(data.begin(), data.end(), std::back_inserter(_data), [](auto& entry) {
      const Skill* data = entry.second;
      QString name = QString::fromLocal8Bit(entry.first.data());

      return std::make_pair(name, data);
    });
  }

  int rowCount(const QModelIndex&) const override { return _data.size(); }
  int columnCount(const QModelIndex&) const override { return 1; }
};

class SkillsTableModel : public DataTableModel<const Skill*>
{
private:
  mutable sprite_cache_t<const Skill*> cache;
  std::array<QString, 2> headers = { { "Identifier" } };

public:
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override
  {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
      return headers[section];

    return QVariant();
  }

  QVariant data(const QModelIndex& i, int role) const override
  { 
    if (role == Qt::DisplayRole && i.column() == 0)
    {
      return _data[i.row()].first;
    }
    else if (role == Qt::DecorationRole && i.column() == 0)
    {
      const Skill* skill = _data[i.row()].second;
      auto it = cache.find(skill);

      if (it == cache.end())
      {
        SpriteInfo info = skill->icon();

        if (info.isLBX())
        {
          const lbx::LBXSpriteData* data = lbx::Repository::spriteFor(info);

          QImage image = QImage(data->width*2, data->height*2, QImage::Format_RGBA8888);

          for (int y = 0; y < image.height(); ++y)
            for (int x = 0; x < image.width(); ++x)
              image.setPixel(QPoint(x, y), Gfx::mainPalette->get(data->at(x/2, y/2, 0, 0)));

            
          QPixmap pixmap = QPixmap::fromImage(image);
          cache.emplace(skill, pixmap);
          return pixmap;
        }
        else
          return QVariant();
      }
      else
        return it->second;
    }

    return QVariant();
  }
};

int main(int argc, char* argv[])
{
  Repository::init();
  lbx::Repository::loadLBXFontsAndPalettes();
  lbx::Repository::loadLBX(LBXID::SPECIAL);
  lbx::Repository::loadLBX(LBXID::SPECIAL2);

  yaml::parse();

  QApplication app(argc, argv);

  SkillsTableModel skillsTableModel = SkillsTableModel();

  QTableView* skillsTable = new QTableView();
  skillsTable->verticalHeader()->setDefaultSectionSize(50);
  skillsTable->setModel(&skillsTableModel);
  skillsTable->setWindowTitle("OpenMoM Editor");

  skillsTable->show();

  return app.exec();
}