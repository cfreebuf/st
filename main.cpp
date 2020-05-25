#include <iostream>
#include "smalltable.hpp"  // ST_TABLE

struct Document {
  std::string docid;
  std::string title;
  int64_t publish_time;
};  // Document

DEFINE_ATTRIBUTE(DOCID, std::string);
DEFINE_ATTRIBUTE(DOCINFO, Document);

typedef ST_TABLE(DOCID, DOCINFO, ST_UNIQUE_KEY(DOCID)) DocInfoTable;

bool GetDocument(DocInfoTable& table, const std::string& docid, Document* doc) {
  auto it = table.seek<DOCID>(docid);
  if (it != (long)NULL) {
    *doc = it->at<DOCINFO>();
    return true;
  }
  return false;
}

// docinfo_table.erase<DOCID>(docid);

void Foreach(DocInfoTable& table, std::function<void(Document&)> callback) {
  DocInfoTable::Iterator it = table.begin();
  for (; it != table.end(); ++it) {
    Document document = it->at<DOCINFO>();
    callback(document);
  }
}

int main() {
  DocInfoTable docinfo_table;
  if (docinfo_table.init() != 0) {
    std::cerr << "Init doc info table error" << std::endl;
    return -1;
  }

  Document doc;
  doc.docid = "123456";
  doc.title = "smalltable";
  doc.publish_time = 123;

  docinfo_table.insert(doc.docid, doc);

  Foreach(docinfo_table, [](Document& doc) {
      std::cout << doc.docid << std::endl;
  });

  return 0;
}
