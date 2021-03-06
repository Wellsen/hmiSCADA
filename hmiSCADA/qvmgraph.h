/*!
\file
\brief Заголовочный файл с описанием класса

Данный файл содержит в себе определение класса QVMGraph
*/

#ifndef QVMGRAPH
#define QVMGRAPH

#include <QStringList>

#include "data.h"

/*!
    \brief QVMGraph - класс  для описания узлов nodes (вершин графа) и дуг arcs между узлами
    \author Roman Sukharev
    \version 1.0
    \date Май 2016 года

    Класс обеспечивает:
    1) добавление, удаление, получение узлов графа (со всеми параметрами)
    2) добавление, удаление, получение дуг графа (со всеми параметрами)
*/

class QVMGraph {
public:

    ////
    /// \brief simpleNode - статический метод для формирования параметров узла
    /// \param name - название узла
    /// \param style - название шаблона отображения
    /// \return Data
    ///
    static Data simpleNode(const QString & name, const QString & style = "") {
        Data node;
        node["name"] = name;
        node["type"] = "node";
        node["style"] = style;
        return node;
    }

    ////
    /// \brief simpleArc - статический метод для формирования параметров дуги
    /// \param from - название исходящей вершины дуги
    /// \param to - название входящей вершины дуги
    /// \return
    ///
    static Data simpleArc(const QString & from, const QString & to) {
        Data arc;
        arc["type"] = "arc";
        arc["from"] = from;
        arc["to"] = to;
        return arc;
    }

    ///
    /// \brief arcName - статический метод для формирования названия дуги
    /// \param fromNode - название узла отправителя
    /// \param toNode - название узла получателя
    /// \return
    ///
    static QString arcName(const QString & fromNode, const QString & toNode) {
        return fromNode+"->"+toNode;
    }

    ////
    /// \brief nodeNames - возвращает имена узлов дуги
    /// \param name - имя дуги
    /// \return QList<QString> имена узлов
    ///
    static QStringList arcNodeNames(const QString & name) {
        return name.split("->");
    }

    static QStringList arcNodeNames(const Data & arc) {
        QStringList nodeNames;
        nodeNames << arc["from"].toString();
        nodeNames << arc["to"].toString();
        return nodeNames;
    }

    ////
    /// \brief insertNode - добавление узла в граф
    /// \param name - название узла
    /// \param style - название шаблона отображения
    ///
    void insertNode(const QString & name, const QString & style = "") {
        if(!name.isEmpty()) {
            m_nodes.insert(name, simpleNode(name, style));
        }
    }

    ////
    /// \brief insertNode - добавление узла в граф
    /// \param node - параметры узла
    ///
    void insertNode(const Data & node) {
        // получение названия узла
        const QString & name = node["name"].toString();
        // если название есть - узел добавляется в граф
        if(!name.isEmpty())
            m_nodes.insert(name, node);
    }

    ////
    /// \brief getNode - получение параметров узла по его названию
    /// \param name - название узла
    /// \return
    ///
    Data getNode(const QString & name) const {
        // получение узла
        Data node = m_nodes[name];

        // если узел отсутствует - возвращается значение по умолчанию
        if(node["name"].isNull())
             node = simpleNode(name);

        return node;
    }

    ////
    /// \brief removeNode - удаление узла по его названию
    /// \param name
    ///
    void removeNode(const QString & name) {
        // удаление узла
        m_nodes.remove(name);

        // удаление связанных с узлом дуг
        const QStringList & arcNames(m_arcs.keys());
        for(int i = 0, max = arcNames.size(); i < max; ++i) {
            const QString & arcName(arcNames[i]);
            if(arcName.contains(name)) {
                const QStringList & nodeNames(arcNodeNames(arcName));
                if((nodeNames.value(0) == name)
                   || (nodeNames.value(1) == name))
                        m_arcs.remove(arcName);
            }
        }
    }

    ////
    /// \brief insertArc - добавление в граф дуги узла from в узел to
    /// \param from - название исходящей вершины дуги
    /// \param to - название входящей вершины дуги
    ///
    void insertArc(const QString & from, const QString & to) {

        if(!(m_nodes[from].isNull() || m_nodes[to].isNull())) {
            m_arcs.insert(arcName(from, to), simpleArc(from, to));
        }
    }

    ////
    /// \brief insertArc - добавление в граф дуги узла from в узел to
    /// \param arc - дуга
    ///
    void insertArc(const Data & arc) {

        QStringList nodeNames = arcNodeNames(arc);
        QString from(nodeNames.at(0)), to(nodeNames.at(1));

        if(!(m_nodes[from].isNull() || m_nodes[to].isNull())) {
            m_arcs.insert(arcName(from, to), arc);
        }
    }

    ////
    /// \brief getArc -  получение параметров дуги по ее названию
    /// \param name
    /// \return
    ///
    Data getArc(const QString & name) const {
        return m_arcs[name];
    }

    ////
    /// \brief getArc - получение параметров дуги по названиям вершин
    /// \param from - название исходящей вершины дуги
    /// \param to - название входящей вершины дуги
    /// \return
    ///
    Data getArc(const QString & from, const QString & to) const {
        return m_arcs[arcName(from, to)];
    }

    ////
    /// \brief removeArc - удаление из графа дуги по названиям вершин
    /// \param from - название исходящей вершины дуги
    /// \param to - название входящей вершины дуги
    ///
    void removeArc(const QString & from, const QString & to) {
        m_arcs.remove(arcName(from, to));
    }

    ////
    /// \brief getNodes - получение параметров всех узлов графа
    /// \return
    ///
    const Data & getNodes() const {
        return m_nodes;
    }

    ////
    /// \brief nodesCount - подсчет количества узлов графа
    /// \return
    ///
    int nodesCount() const {
        return m_nodes.size();
    }

    ////
    /// \brief getArcs - получение параметров всех дуг графа
    /// \return
    ///
    const Data & getArcs() const {
        return m_arcs;
    }

    ////
    /// \brief arcsCount - подсчет количества дуг графа
    /// \return
    ///
    int arcsCount() const {
        return m_arcs.size();
    }

protected:

    ////
    /// \brief nodes - узлы графа
    ///
    Data m_nodes;

    ////
    /// \brief arcs - дуги графа
    ///
    Data m_arcs;
};


#endif // QVMGRAPH

