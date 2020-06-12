#include <algorithm>

#include <QtWidgets/QApplication>
#include <QtWidgets/QTableView>

#include <QtCore/QAbstractTableModel>

#include "gfx/LBXRepository.h"
#include "save/YAML.h"

#include "yaml-cpp/node/detail/node_data.h"
std::string YAML::detail::node_data::empty_scalar;

using namespace lbx;

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
  QVariant data(const QModelIndex& i, int role) const override
  { 
    if (role == Qt::DisplayRole)
    {      
      return _data[i.row()].first;
    }

    return QVariant();
  }
};

int main(int argc, char* argv[])
{
  Repository::init();
  yaml::parse();

  QApplication app(argc, argv);

  SkillsTableModel skillsTableModel = SkillsTableModel();

  QTableView* skillsTable = new QTableView();
  skillsTable->setModel(&skillsTableModel);
  skillsTable->setWindowTitle("OpenMoM Editor");

  skillsTable->show();

  return app.exec();
}