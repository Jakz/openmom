#include "Race.h"

#include "Localization.h"

const Race Race::races[] = {
  {
    RaceID::BARBARIANS,I18::RACE_NAME_BARBARIANS, I18::UNIT_NAME_BARBARIANS, HOUSE_MUD, 20, 0.15f,
    {"Norport","Bradenburg","Bromburg","Bamburg","Burglitz","Deventor","Danzig","Freiburg","Flensburg","Hamburg","Hannover","Konstanz","Kufstein","Linz","Strassburg","Rostock","Schleswig","Stettin","Zwolle","Soest"}
  },
  {
    RaceID::BEASTMEN,I18::RACE_NAME_BEASTMEN, I18::UNIT_NAME_BEASTMEN, HOUSE_MUD, 0, 0.05f,
    {"Kempen","Bantanu","Saradash","Tanquil","Jagan","Panjang","Mourndar","Turlock","Tetrehahn","Fortalle","Tier","Raban","Kraang","Amondon","Carn Sul","Mijnduin","Durwrath","Lorn","Caradas","Rastenford"}
  },
  {
    RaceID::DARK_ELVES,I18::RACE_NAME_DARK_ELVES, I18::UNIT_NAME_DARK_ELVES, HOUSE_MUD, -20, 0.02f,
    {"Leer","Nightwath","Tyrlinith","Blademarsh","Bloodrock","Darkhold","Darkmoor","Hellgate","Bladewater","Tombstone","Midnight","Jet","Ebony","Fellwood","Ashwall","Thanelorn","Anamarathor","Deathford","Blackstone","Adderax"}
  },
  {
    RaceID::DRACONIANS,I18::RACE_NAME_DRACONIANS, I18::UNIT_NAME_DRACONIANS, HOUSE_MUD, -10, 0.05f,
    {"Vallis","Minnlow","Mishran","Alairium","Dragon Pass","Doomcastle","Solanar","Farmist","Dragonfall","Parch","Arie","Wynd","Firepeak","Azurtfall","Fangrock","High Top","Hillfang","Sorin","Straatus","Icetop"}
  },
  {
    RaceID::DWARVES,I18::RACE_NAME_DWARVES, I18::UNIT_NAME_DWARVES, HOUSE_MUD, -20, 0.07f,
    {"Ebonsway","Ultigar","Pyre","Asengard","Stonegate","Blade Stone","Rockwedge","Abanax","Ailearth","Fire Keep","Asbraun","Axhome","Iron Wall","Raug Wall","Stonehome","Finnrock","Southold","Stony Peak","Crag Keep","Hearth"}
  },
  {
    RaceID::GNOLLS,I18::RACE_NAME_GNOLLS, I18::UNIT_NAME_GNOLLS, HOUSE_MUD, -10, 0.05f,
    {"Basel","Aachen","Braunweig","Breslau","Brunn","Dortmond","Maimz","Nymax","Ozenwall","Posen","Speger","Steyr","Trier","Vordinax","Jogrin","Geras","Megamoria","Melegok","Tiamut","Creymona"}
  },
  {
    RaceID::HALFLINGS,I18::RACE_NAME_HALFLINGS, I18::UNIT_NAME_HALFLINGS, HOUSE_MUD, 0, 0.15f,
    {"Miroban","Eastway","Palaor","Elyndendale","Cornwell","Jollyville","Hickorydale","Hialanth","Ethridge","Myrean","Mirndale","Honeypool","Eventon","Sunnydale","Bremen","Sacred Vale","Medowdale","Cornberry","Hillshire","Torwell"}
  },
  {
    RaceID::HIGH_ELVES,I18::RACE_NAME_HIGH_ELVES, I18::UNIT_NAME_HIGH_ELVES, HOUSE_MUD, -20, 0.05f,
    {"Silverdale","Esterwood","Rivendale","Alm","Arissawood","Karsol","Prose","Fairwall","Arairia","Lylros","Lanerell","Venagreen","Rosewood","Ashenwood","Wooddale","Avalon","Elmwood","Glendale","Timberland","Gatewood"}
  },
  {
    RaceID::HIGH_MEN,I18::RACE_NAME_HIGH_MEN, I18::UNIT_NAME_HIGH_MEN, HOUSE_MUD, 0, 0.10f,
    {"Coventry","York","Nottingham","Birmingham","Dover","Liverpool","Brighton","Oxford","Reading","Exeter","Cambridge","Hastings","Cantebury","Banbury","Newcastle","Haven","New Haven","Bremen","Denislin","Newland"}
  },
  {
    RaceID::KLACKONS,I18::RACE_NAME_KLACKONS, I18::UNIT_NAME_KLACKONS, HOUSE_MUD, -20, 0.05f,
    {"Fa-rul","Niktock","Ixtu","Keltok","Hsi-raz","Cai-ru","Katax","Ni-aru","Chu-wak","Ty-go","Aiym","Tar-reash","Shen-khi","Li-ray","Braxus","Pangor","Thirock","Zho","Tartz","Rakutal"}
  },
  {
    RaceID::LIZARDMEN,I18::RACE_NAME_LIZARDMEN, I18::UNIT_NAME_LIZARDMEN, HOUSE_MUD, 10, 0.10f,
    {"South Wash","Thorn","Ulm","Ulna","Lizard Caves","Longrift","Lizardmarch","Nar","Mudspray","Sawtooth","Gore","Dirage","Sludgehome","Sedge","Rashem","Hob","Shatterod","Scacrest","Shellford","Hornbrant"}
  },
  {
    RaceID::NOMADS,I18::RACE_NAME_NOMADS, I18::UNIT_NAME_NOMADS, HOUSE_MUD, -10, 0.10f,
    {"Mecca","Naples","Sidon","Tyre","Tarsus","Issus","Cunaxa","Cremona","Cannae","Capua","Torin","Genoa","Utica","Damascus","Verona","Salamis","Blake","Harold","Isseme","Doraat"}
  },
  {
    RaceID::ORCS,I18::RACE_NAME_ORCS, I18::UNIT_NAME_ORCS, HOUSE_MUD, 0, 0.10f,
    {"Robenaar","Wrrmwood","Xanten","Charack","Largut","Bangaak","Darkpool","Golcanar","Skulltop","Gotartop","Zuul","Craagpool","Krong","Krag Pool","Fell Gorge","Gronk","Torogaar","Oxenaak","Zom","Rife"}
  },
  {
    RaceID::TROLLS,I18::RACE_NAME_TROLLS, I18::UNIT_NAME_TROLLS, HOUSE_MUD, -20, 0.03f,
    {"Erfurt","Gorfite","Goslar","Graz","Groningen","Koblenz","Mainz","Fauul","Giant Peaks","Craagut","Bryn","Rothiem","Phobas","Dang","Zorag","Undermarsh","Rotwood","Pyth","Muck","Deathmound"}
  }
};