#include <algorithm>
#include <unordered_map>

#include <QtWidgets/QApplication>
#include <QtWidgets/QTableView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QStyleFactory>

#include <QtCore/QAbstractTableModel>

#include "gfx/LBXRepository.h"
#include "save/YAML.h"
#include "ui/GfxData.h"

#include "yaml-cpp/node/detail/node_data.h"
std::string YAML::detail::node_data::empty_scalar;

#define TO_QSTR(x) QString::fromLocal8Bit(x.c_str())

Q_DECLARE_METATYPE(skills::Type);

enum
{
  melee = LBXI(COMPIX, 61),
  defense = LBXI(COMPIX, 70),
  resistance = LBXI(COMPIX, 75),
  hits = LBXI(COMPIX, 71),
  ranged_bow = LBXI(COMPIX, 66),
  movement_ground = LBXI(COMPIX, 72),
  gold = LBXI(BACKGRND, 42),
  mana = LBXI(BACKGRND, 43)

};

template<typename T>
class enum_name_map_t
{
private:
  using map_t = std::unordered_map<std::string, T>;
  mutable map_t data;

public:
  enum_name_map_t(const std::initializer_list<typename map_t::value_type>& data) : data(data) { }

  T operator[](const std::string& key) const { return data[key]; }

  const std::string& forValue(T val) const
  {
    for (const auto& e : data)
      if (e.second == val)
        return e.first;

    return nullptr;
  }

  auto begin() const { return data.begin(); }
  auto end() const { return data.end(); }

  size_t size() const { return data.size(); }
};

static const enum_name_map_t<skills::Type> skillTypeMap = enum_name_map_t<skills::Type>({
  { "native", skills::Type::NATIVE },
  { "hero", skills::Type::HERO },
  { "spell", skills::Type::SPELL },
  { "item_power", skills::Type::ITEM_POWER },
  { "combat_effect", skills::Type::COMBAT_EFFECT }
});


using namespace lbx;

struct sprite_info_hash
{
  size_t operator()(const SpriteInfo& info) const { return *((u32*)(void*)&info); } //TODO: totally unsafe for now
};

template<typename T> using sprite_cache_t = std::unordered_map<T, QPixmap, sprite_info_hash>;
sprite_cache_t<SpriteInfo> cache;

QPixmap cacheGet(SpriteInfo info)
{
  auto it = cache.find(info);

  if (it != cache.end())
    return it->second;
  else if (!info.isLBX())
    return QPixmap();
  else
  {
    const lbx::LBXSpriteData* data = lbx::Repository::spriteFor(info);

    QImage image = QImage(data->width * 2, data->height * 2, QImage::Format_RGBA8888);

    for (int y = 0; y < image.height(); ++y)
      for (int x = 0; x < image.width(); ++x)
      {
        auto ci = data->at(x / 2, y / 2, 0, 0);
        if (ci != 0)
          image.setPixel(QPoint(x, y), Gfx::mainPalette->get(ci));
        else
          image.setPixel(QPoint(x, y), 0);
      }

    QPixmap pixmap = QPixmap::fromImage(image);
    cache.emplace(info, pixmap);

    return pixmap;
  }
}

template<typename T>
class DataTableModel : public QAbstractTableModel
{
protected:
  std::vector<std::pair<QString, const T*>> _data;

public:
  DataTableModel()
  {
    const auto& data = Data::values<const T*>();

    auto it = data.begin();

    std::transform(data.begin(), data.end(), std::back_inserter(_data), [](auto& entry) {
      const T* data = entry.second;
      QString name = TO_QSTR(entry.first);

      return std::make_pair(name, data);
    });
  }

  int rowCount(const QModelIndex&) const override { return _data.size(); }
};

class EnumItemDelegate : public QStyledItemDelegate
{
public:
  QWidget *createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override
  {
    QComboBox* cb = new QComboBox(parent);

    for (const auto& e : skillTypeMap)
      cb->addItem(TO_QSTR(e.first));

    return cb;
  }
  
  void setEditorData(QWidget* editor, const QModelIndex& index) const override
  {
    QComboBox* cb = qobject_cast<QComboBox*>(editor);
    //index.data(Qt::EditRole);
    cb->setCurrentIndex(1);
  }

  void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override
  {
    QComboBox *cb = qobject_cast<QComboBox*>(editor);
    
  }
};

class SkillsTableModel : public DataTableModel<Skill>
{
private:
  std::array<QString, 2> headers = { { "Identifier", "Type" } };

public:
  int columnCount(const QModelIndex&) const override { return headers.size(); }

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override
  {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
      return headers[section];

    return QVariant();
  }

  Qt::ItemFlags flags(const QModelIndex& index) const
  {
    Qt::ItemFlags flags = DataTableModel<Skill>::flags(index);
    
    if (index.column() == 1) flags |= Qt::ItemIsEditable;

    return flags;
  }

  bool setData(const QModelIndex& index, const QVariant& value, int role) override
  {
    // const Skill* skill = _data[index.row()].second;
    // printf("Type: %d", (int)skillTypeMap[(const char*)value.toString().toLocal8Bit()]);
    return true;
  }

  QVariant data(const QModelIndex& i, int role) const override
  { 
    const Skill* skill = _data[i.row()].second;
    
    if (role == Qt::DisplayRole)
    {
      switch (i.column())
      {
      case 0: return _data[i.row()].first;
      case 1: return TO_QSTR(skillTypeMap.forValue(skill->type()));
      }
    }
    else if (role == Qt::DecorationRole && i.column() == 0)
    {
      SpriteInfo icon = skill->icon();
      return cacheGet(icon);
    }

    return QVariant();
  }
};

class UnitTableModel : public DataTableModel<UnitSpec>
{
private:
  std::array<QString, 10> headers = { { "Identifier", "Upkeep", "Cost", "Melee", "Ranged", "Defense", "Resistance", "Hits", "Figures", "Movement" } };  

public:
  int columnCount(const QModelIndex&) const override { return headers.size(); }
  
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override
  {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
      return headers[section];

    return QVariant();
  }

  QVariant data(const QModelIndex& i, int role) const override
  {
    const UnitSpec* unit = _data[i.row()].second;

    if (role == Qt::DisplayRole)
    {
      switch (i.column())
      {
      case 0: return _data[i.row()].first;
      case 1: return unit->type == UnitType::FANTASTIC ? unit->productionUpkeep().mana : unit->productionUpkeep().gold;
      case 2: return unit->cost;
      case 3: return unit->melee;
      case 4: return unit->ranged.strength;
      case 5: return unit->defense;
      case 6: return unit->resistance;
      case 7: return unit->hits;
      case 8: return unit->figures;
      case 9: return unit->movement;
      }
    }
    else if (role == Qt::DecorationRole)
    {
      switch (i.column())
      {
        case 0:
        {
          const UnitGfxSpec& gfx = GfxData::unitGfx(unit);
          auto info = gfx.still;
          return cacheGet(info);
        }
        case 1: 
        case 2:
          return unit->type == UnitType::FANTASTIC ? cacheGet(mana) : cacheGet(gold);
        case 3: return cacheGet(melee);
        case 4: break;
        case 5: return cacheGet(defense);
        case 6: return cacheGet(resistance);
        case 7: return cacheGet(hits);
        case 8: break;
        case 9: return cacheGet(movement_ground);
      }
    }
    else if (role == Qt::TextAlignmentRole)
    {
      switch (i.column())
      {
      case 0: return Qt::AlignVCenter;
      default: return Qt::AlignCenter;
      }
    }

    return QVariant();
  }
};

int main(int argc, char* argv[])
{
  Repository::init();
  lbx::Repository::loadLBXFontsAndPalettes();
  lbx::Repository::loadLBX(LBXID::BACKGRND);
  lbx::Repository::loadLBX(LBXID::COMPIX);
  lbx::Repository::loadLBX(LBXID::SPECIAL);
  lbx::Repository::loadLBX(LBXID::SPECIAL2);
  lbx::Repository::loadLBX(LBXID::UNITS1);
  lbx::Repository::loadLBX(LBXID::UNITS2);

  yaml::parse();

  QApplication app(argc, argv);

  SkillsTableModel skillsTableModel = SkillsTableModel();

  struct TableInfo
  {
    QAbstractTableModel* model;
    std::string name;
    int height;
    QTableView* view;
  };

  std::array<TableInfo, 2> tables = { {
    { new SkillsTableModel(), "Skills", 50, nullptr },
    { new UnitTableModel(), "Units", 50, nullptr }
  } };

  QTabWidget* tabPane = new QTabWidget();
  tabPane->setTabPosition(QTabWidget::TabPosition::West);
  tabPane->setMinimumSize(QSize(640, 480));

  for (auto& entry : tables)
  {
    QTableView* tableView = new QTableView();
    tableView->verticalHeader()->setDefaultSectionSize(50);
    tableView->setModel(entry.model);
    tabPane->addTab(tableView, entry.name.c_str());
    entry.view = tableView;
  }

  tables[1].view->setColumnWidth(0, 200);
  for (int i = 1; i <= 9; ++i)
    tables[1].view->setColumnWidth(i, 50);

  tabPane->setWindowTitle("OpenMoM Editor");
  tabPane->show();

  return app.exec();
}