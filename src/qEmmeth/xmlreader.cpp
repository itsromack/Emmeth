/*********************
 * xmlReader Library *
 * Benjamin Schnabel *
 * (c) 2015          *
 * The xmlReader is  *
 * based on QXMLStreamReader   *
 *********************/

#include "xmlreader.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QIcon>
#include <QTextDocument>
#include <QVector>
//#include <QtXmlPatterns/QXmlSchema>
//#include <QtXmlPatterns/QXmlSchemaValidator>
#include "jsonparser.h"



xmlReader::xmlReader()
{

}

xmlReader::~xmlReader()
{

}

QString xmlReader::load(QString fileName){
    qDebug() << "reading xml file";

    QFile xmlFile(fileName);
    xmlFile.open(QIODevice::ReadOnly|QIODevice::Text);

    QXmlStreamReader xml;
    xml.setDevice(&xmlFile);
    //qDebug() << "Encoding: " + xml.documentEncoding().toString();

    //TODO: read through the josn parser.
    //qDebug() << "jsonParser started...";
    //jsonParser *jsonParse = new jsonParser();
    //jsonParse->load(":/assets/xmlschemas/assets/xmlschemas/tanach-xml.json");

    QString result;
    QTextDocument *document = new QTextDocument;
    //TODO: make this a rtf document, formatted
    //while(xml.readNextStartElement()){
    xml.readNext();
    while(!xml.atEnd() ){ //&& !xml.hasError()){
        //qDebug() << xml.name();
        xml.readNextStartElement();
        if(xml.isStartElement()){
            // if(xml.name() == "book"){ //look for the beginning of the document
            //qDebug() << xml.name();
            //result += xml.text().toString();
            //}

            if(xml.name()== "w"){ //read without new line
                QString w = xml.readElementText(QXmlStreamReader::SkipChildElements);
                //remove slashes form the text
                result += w.remove("/");
                //remove space after Linae Mappekh
                if(!w.endsWith("־")){
                    result += " ";
                }
            }
            //}


            if(xml.name() == "c"){ //if chapter found display it
                QXmlStreamAttributes attr = xml.attributes();
                result += "<a><h1 dir=\"rtl\" style=\"color:blue\">";
                result += attr.first().value().toString();
                result += "</h1></a>";
                result += "\n";
            }
            if(xml.name() == "v"){
                QXmlStreamAttributes attr = xml.attributes();
                result += "<a><b style=\"color:blue\">";
                result += attr.first().value().toString();
                result += "</b></a>";
                result += " ";
            }
            if(xml.name() == "pe"){
                result += " <b>פ</b><br />";
            }
            if(xml.name() == "samekh"){
                result += "<b>ס</b> ";
            }
        } else { xml.readNext();}
    }

    if (xml.hasError()) {
        // do error handling
        QMessageBox::critical(0, "Error",xml.errorString(),QMessageBox::Close);
        xml.raiseError();
        qDebug() << xml.errorString();
        qDebug() << xml.error();
    }

    //qDebug() << "Result: " + result;
    xmlFile.close();
    return result;
}

QString xmlReader::loadInfo(QString fileName){
    //book info, displays the additional content & information for a book
    qDebug() << "Load Info";

    //QFileInfo fi(fileName);
    qDebug() << fileName;
    QFile xmlFile(fileName);
    //qDebug() << fi.completeBaseName();
    xmlFile.open(QIODevice::ReadOnly|QIODevice::Text);

    QXmlStreamReader xml;
    xml.setDevice(&xmlFile);

    QString result;
    qDebug() << "Encoding: " + xml.documentEncoding().toString();

    while( !xml.atEnd() && !xml.hasError()){
        xml.readNext();

        //qDebug() << xml.name();
        //qDebug() << xml.tokenString().toUtf8();
        //qDebug() << xml.text().toString();
        //qDebug() << xml.tokenString();
        if(xml.isStartDocument()){
            if(xml.name() == "teiHeader"){
                //result += xml.name().toString();
                result += xml.readElementText();
                //qDebug() << xml.tokenString();

                //qDebug() << "teiHeader found.";
                //Debug() << xml.tokenString().toUtf8();
            }
        }
    }
    if(xml.hasError()){
        xml.raiseError();
        qDebug() << "Error in xml file";
    }
    return result;

}

bool xmlReader::validate(QString fileName){
    bool result = false;
    //QXmlSchema schema;
    //schema.load(QUrl(fileName));

    //if ( schema.isValid() ) {
    //    QXmlSchemaValidator validator( schema );
    //    if ( validator.validate( QUrl(fileName) ) ) {
    //        qDebug() << "instance is valid";
    //        result = true;
    //    } else {
    //        qDebug() << "instance is invalid";
    //    }
    //} else {
    //    qDebug() << "schema is invalid";
    //}
    return result;

}

QStringList xmlReader::readChapterVerses(QString fileName){
    qDebug() << "reading chapter and verses";

    //QFileInfo fi(fileName);
    QFile xmlFile(fileName);
    xmlFile.open(QIODevice::ReadOnly|QIODevice::Text);

    QXmlStreamReader xml;
    xml.setDevice(&xmlFile);

    QStringList result;
    //QString resChapter;
    QString resVerse;

    while( !xml.atEnd() && !xml.hasError()){
        xml.readNext();
        if(xml.isStartElement()){
            // do processing
            if(xml.name() == "c"){
                QXmlStreamAttributes chapter = xml.attributes();
                qDebug() << "chapter: " + chapter.first().value().toString();
            }
            if(xml.name() == "v"){
                QXmlStreamAttributes verse = xml.attributes();
                qDebug() << "Verse: " + verse.first().value().toString();
                resVerse = verse.first().value().toString();
            }
        }
        if(xml.isEndElement()){
            if(xml.name() == "c"){
                result << resVerse;
            }
        }
    }

    if (xml.hasError()) {
        // do error handling
        xml.raiseError();
        qDebug() << xml.error();
    }

    qDebug() << result;
    return result;
}

QString xmlReader::readBookTitle(QString fileName){
    qDebug() << "reading book title";

    QFile xmlFile(fileName);
    xmlFile.open(QIODevice::ReadOnly|QIODevice::Text);

    QXmlStreamReader xml;
    xml.setDevice(&xmlFile);

    QString result;

    while( !xml.atEnd() && !xml.hasError()){
        xml.readNext();
        if(xml.isStartElement()){
            // do processing
            if(xml.name() == "title"){
                QXmlStreamAttributes &attr = xml.attributes();
                QString level = attr.value("level").toString();
                QString type = attr.value("type").toString();

                if((level == "a") && (type == "main")){
                    result = xml.readElementText();
                }
            }
        }

    }

    if (xml.hasError()) {
        // do error handling
        xml.raiseError();
        qDebug() << xml.error();
    }

    qDebug() << "result: " + result;
    return result;
}
//TODO: tanach-xml full
//TODO: read multiple XML Versions
//TODO: Zefania XML
//TODO: XSEM from SIL
//TODO: OSIS http://www.bibletechnologies.net/ http://www.crosswire.org/study/
//TODO: ThML http://www.ccel.org/ThML/
//TODO: http://ebible.org/bible/gbf.htm http://ebible.org/translation/
//TODO: Mechon Mamre as html
//TODO: Develop Emmeth XML Markup language
//TODO: Unified Scripture Format XML (USFX)
//TODO: TEI http://www.tei-c.org/index.xml
//TODO: SWORD Project http://www.crosswire.org/sword/index.jsp
//TODO: STEP https://www.stepbible.org/



