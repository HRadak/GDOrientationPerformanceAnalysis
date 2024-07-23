/**############################################################################
#
#
# Copyright (C) 2021 by Christian Scheunert
#
# Email: christian.scheunert@tu-dresden.de
#
#############################################################################*/

/**############################################################################
 # INCLUDES
 #############################################################################*/

#include "ioconfigfile.h"

/**############################################################################
 # GLOBALS
 #############################################################################*/

/**############################################################################
 # LOCAL DEFINITIONS
 #############################################################################*/

/**############################################################################
 # MEMBER DEFINITIONS
 #############################################################################*/

/******************************************************************************
 *****************************************************************************/
IoConfigFileExcept::IoConfigFileExcept(int _num) :
		IoFileExcept(_num) {

}

/******************************************************************************
 *****************************************************************************/
IoConfigFile::IoConfigFile(const string &_fileName, const bool &_sortContent) :
		IoSectionKeyFile(_fileName)

{
	if (_fileName == "") {
		fileName = _fileName;
		sortContent = _sortContent;
		changedData = false;
	} else {
		loadFile(_fileName, _sortContent);
	}
}

/******************************************************************************
 *****************************************************************************/
IoConfigFile::~IoConfigFile() {

	try {
		if (changedData) {
			saveFile();
		}
	}

	catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::~IoConfigFile" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
IoConfigFile::Section* IoConfigFile::getSection(const string &_section) {
	unsigned long i;

	for (i = 0; i < sectionList.size(); i++) {
		if (_section.compare(sectionList[i].name) == 0) {
			return (&sectionList[i]);
		}
	}
	return (0);
}

/******************************************************************************
 *****************************************************************************/
IoConfigFile::Key* IoConfigFile::getKey(const string &_key,
		const string &_section) {
	unsigned long i;
	Section *p_section;

	if ((p_section = getSection(_section)) == 0) {
		return (0);
	}
	for (i = 0; i < p_section->keyList.size(); i++) {
		if (_key.compare(p_section->keyList[i].name) == 0) {
			return (&(p_section->keyList[i]));
		}
	}
	return (0);
}

/******************************************************************************
 *****************************************************************************/
IoConfigFile::Section* IoConfigFile::createSection(const string &_section) {
	Section section;
	vector<Section>::iterator i;

	try {
		if ((getSection(_section)) != NULL) {
			throw IoConfigFileExcept(ERROR_SECTION_ALREADY_EXISTS);
		}
		section.name = _section;
		section.emptyLines = 0;
		if ((sectionList.size() > 0) && (sortContent)) {
			for (i = sectionList.begin(); i < sectionList.end(); i++) {
				if (_section.compare(i->name) < 0) {
					changedData = true;
					sectionList.insert(i, section);
					return (&(*i));
				}
			}
		}
		sectionList.push_back(section);
		changedData = true;
		return (&sectionList.back());
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_SECTION_ALREADY_EXISTS) {
			cerr << "ERROR : SECTION_ALREADY_EXISTS : ";
			cerr << "section = " << convSectionName(_section) << " : ";
		}
		cerr << "IoConfigFile::createSection" << endl;
		throw;
	} catch (bad_alloc &_e) {
		cerr << "ERROR : COULD_NOT_CREATE_SECTION : ";
		cerr << "IoConfigFile::createSection" << endl;
		throw IoConfigFileExcept(ERROR_COULD_NOT_CREATE_SECTION);
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::createSection" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
IoConfigFile::Key* IoConfigFile::createKey(const string &_key,
		const string &_section) {
	Key key;
	Section *p_section;
	vector<Key>::iterator i;

	try {
		if (_key == "") {
			throw IoConfigFileExcept(ERROR_MISSING_KEY_NAME);
		}
		if ((getKey(_key, _section)) != NULL) {
			throw IoConfigFileExcept(ERROR_KEY_ALREADY_EXISTS);
		}
		if ((p_section = getSection(_section)) == NULL) {
			p_section = createSection(_section);
		}
		key.name = _key;
		key.emptyLines = 0;
		if ((p_section->keyList.size() > 0) && (sortContent)) {
			for (i = p_section->keyList.begin(); i < p_section->keyList.end(); i++) {
				if (_key.compare(i->name) < 0) {
					changedData = true;
					p_section->keyList.insert(i, key);
					return (&(*i));
				}
			}
		}
		p_section->keyList.push_back(key);
		changedData = true;
		return (&p_section->keyList.back());
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_MISSING_KEY_NAME) {
			cerr << "ERROR : MISSING_KEY_NAME : ";
		}
		if (_e.num == ERROR_KEY_ALREADY_EXISTS) {
			cerr << "ERROR : KEY_ALREADY_EXISTS : ";
			cerr << "section = " << convSectionName(_section) << " : ";
			cerr << "key = " << _key << " : ";
		}
		cerr << "IoConfigFile::createKey" << endl;
		throw;
	} catch (bad_alloc &_e) {
		cerr << "ERROR : COULD_NOT_CREATE_KEY : ";
		cerr << "IoConfigFile::createKey" << endl;
		throw IoConfigFileExcept(ERROR_COULD_NOT_CREATE_KEY);
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::createKey" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string IoConfigFile::extractKey(const string &_string) {
	string key;
	int charPos;

	try {
		charPos = _string.find_first_of(EQUAL_INDICATORS);
		if (charPos > -1) {
			key = _string.substr(0, charPos);
		} else {
			throw IoConfigFileExcept(ERROR_MISSING_EQUAL_INDICATOR);
		}
		trim(key, WHITE_SPACE);
		return (key);
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_MISSING_EQUAL_INDICATOR) {
			cerr << "ERROR : MISSING_EQUAL_INDICATOR : ";
			cerr << _string << " : ";
		}
		cerr << "IoConfigFile::extractKey" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::extractKey" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string IoConfigFile::extractValue(const string &_string) {
	string value;
	int charPos;

	try {
		charPos = _string.find_first_of(EQUAL_INDICATORS);
		if (charPos > -1) {
			value = _string.substr(charPos + 1, _string.size());
		} else {
			throw IoConfigFileExcept(ERROR_MISSING_EQUAL_INDICATOR);
		}
		trim(value, WHITE_SPACE);
		return (value);
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_MISSING_EQUAL_INDICATOR) {
			cerr << "ERROR : MISSING_EQUAL_INDICATOR : ";
			cerr << _string << " : ";
		}
		cerr << "IoConfigFile::extractValue" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::extractValue" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void IoConfigFile::separateValues(valarray<string> &_values,
		const string &_valueString) {
	string valueString;
	long pos;
	unsigned long i;

	try {
		valueString = _valueString;
		if (-1 == (pos = valueString.find(MULTI_VALUE_PREFIX))) {
			_values.resize(1);
			_values[0] = valueString;
			return;
		}
		valueString.erase(0, pos + 1);
		if (-1 == (pos = valueString.find(MULTI_VALUE_POSTFIX))) {
			throw IoConfigFileExcept(ERROR_COULD_NOT_FIND_MULTI_VALUE_POSTFIX);
		}
		i = 1;
		pos = 0;
		while (-1 < (pos = valueString.find(MULTI_VALUE_SEPARATOR, pos))) {
			pos++;
			i++;
		}
		_values.resize(i);
		i = 0;
		while (-1 < (pos = valueString.find(MULTI_VALUE_SEPARATOR))) {
			_values[i].assign(valueString, 0, pos);
			trim(_values[i], WHITE_SPACE);
			valueString.erase(0, pos + 1);
			i++;
		}
		pos = valueString.find(MULTI_VALUE_POSTFIX);
		_values[i].assign(valueString, 0, pos);
		trim(_values[i], WHITE_SPACE);
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_COULD_NOT_FIND_MULTI_VALUE_POSTFIX) {
			cerr << "ERROR : COULD_NOT_FIND_MULTI_VALUE_POSTFIX : ";
		}
		cerr << "IoConfigFile::separateValues" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::separateValues" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void IoConfigFile::setSectionComments(const vector<string> &_comments,
		const unsigned long &_emptyLines, const string &_section) {
	Section *p_section;

	try {
		if ((p_section = getSection(_section)) == 0) {
			throw IoConfigFileExcept(ERROR_SECTION_DOES_NOT_EXIST);
		}
		p_section->emptyLines = _emptyLines;
		p_section->comments.resize(_comments.size());
		p_section->comments = _comments;
		changedData = true;
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_SECTION_DOES_NOT_EXIST) {
			cerr << "ERROR : SECTION_DOES_NOT_EXIST : ";
			cerr << "section = " << convSectionName(_section) << " : ";
		}
		cerr << "IoConfigFile::setSectionComments" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::setSectionComments" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void IoConfigFile::setKeyComments(const vector<string> &_comments,
		const unsigned long &_emptyLines, const string &_key,
		const string &_section) {
	Key *p_key;

	try {
		if ((p_key = getKey(_key, _section)) == 0) {
			throw IoConfigFileExcept(ERROR_KEY_DOES_NOT_EXIST);
		}
		p_key->emptyLines = _emptyLines;
		p_key->comments.resize(_comments.size());
		p_key->comments = _comments;
		changedData = true;
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_KEY_DOES_NOT_EXIST) {
			cerr << "ERROR : KEY_DOES_NOT_EXIST : ";
			cerr << "section = " << convSectionName(_section) << " : ";
			cerr << "key = " << _key << " : ";
		}
		cerr << "IoConfigFile::setKeyComments" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::setKeyComments" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
string& IoConfigFile::formatComment(string &_comment) {
	if (_comment.find_first_of(COMMENT_INDICATORS) == 0) {
		if (_comment[0] == COMMENT_INDICATORS[0]) {
			return (_comment);
		} else {
			_comment = _comment.substr(1, _comment.size());
		}
	}
	trim(_comment, WHITE_SPACE);
	_comment = " " + _comment;
	_comment = COMMENT_INDICATORS.substr(0, 1) + _comment;
	return (_comment);
}

/******************************************************************************
 *****************************************************************************/
void IoConfigFile::setStringValues(const valarray<string> &_values,
		const string &_key, const string &_section) {
	Key *p_key;
	unsigned long i;

	try {
		/** get the key in section specified *************************************/
		if ((p_key = getKey(_key, _section)) == 0) {
			p_key = createKey(_key, _section);
		}

		/** set the output strings ***********************************************/
		if (1 < _values.size()) {
			p_key->value = MULTI_VALUE_PREFIX;
			for (i = 0; i < _values.size(); i++) {
				if (_values[i] == "") {
					throw IoConfigFileExcept(ERROR_EMPTY_VALUE_STRING);
				}
				p_key->value += _values[i];
				if (i < (_values.size() - 1)) {
					p_key->value += MULTI_VALUE_SEPARATOR;
					p_key->value += " ";
				} else {
					p_key->value += MULTI_VALUE_POSTFIX;
				}
			}
		} else {
			if (_values[0] == "") {
				throw IoConfigFileExcept(ERROR_EMPTY_VALUE_STRING);
			}
			p_key->value = _values[0];
		}
		changedData = true;
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_EMPTY_VALUE_STRING) {
			cerr << "ERROR : EMPTY_VALUE_STRING : ";
		}
		cerr << "IoConfigFile::setStringValues" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::setStringValues" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void IoConfigFile::getStringValues(valarray<string> &_values,
		const string &_key, const string &_section, const unsigned long &_size) {
	Key *p_key;

	try {
		if ((p_key = getKey(_key, _section)) == 0) {
			throw IoConfigFileExcept(ERROR_KEY_DOES_NOT_EXIST);
		}
		separateValues(_values, p_key->value);
		if (_size != 0) {
			if (_size != _values.size()) {
				throw IoConfigFileExcept(ERROR_WRONG_NUMBER_OF_VALUES);
			}
		}
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_KEY_DOES_NOT_EXIST) {
			cerr << "ERROR : KEY_DOES_NOT_EXIST : ";
		} else if (_e.num == ERROR_WRONG_NUMBER_OF_VALUES) {
			cerr << "ERROR : WRONG_NUMBER_OF_VALUES : ";
			cerr << "expected = " << _size << " : ";
			cerr << "found = " << _values.size() << " : ";
		} else {
			cerr << "ERROR : UNKNOWN : ";
		}
		cerr << "section = " << convSectionName(_section) << " : ";
		cerr << "key = " << _key << " : ";
		cerr << "IoConfigFile::getStringValues" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "section = " << convSectionName(_section) << " : ";
		cerr << "key = " << _key << " : ";
		cerr << "IoConfigFile::getStringValues" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void IoConfigFile::loadFile(const string &_fileName, const bool &_sortContent) {
	string key = "";
	string value = "";
	string section = "";
	unsigned long emptyLines = 0;
	vector<string> comments;

	try {
		/** prepare for raeding the config file **********************************/
		clear();
		setFileName(_fileName);
		sortContent = _sortContent;
		openFileForRead();

		/** create a section of empty name to store the config file header *******/
		createSection(section);

		/** read the config file *************************************************/
		while (!endOfFile()) {
			readLine(line);

			/************************************************************************
			 * Firstly make sure we do not loose the config file header and secondly
			 * clear all comments before a empy line if we got the header already.
			 ***********************************************************************/
			if (line == "") {
				if (0 < comments.size()) {
					if (sectionList.size() == 1) {
						if (sectionList[sectionList.size() - 1].keyList.size() == 0) {
							if (sectionList[sectionList.size() - 1].comments.size() == 0) {
								setSectionComments(comments, emptyLines, section);
							}
						}
					}
					comments.clear();
					emptyLines = 0;
				}
				emptyLines++;
			}

			/** run comment specific operations ************************************/
			else if (line.find_first_of(COMMENT_INDICATORS) == 0) {
				comments.push_back(formatComment(line));
			}

			/** run section specific operations ************************************/
			else if (line.find_first_of(SECTION_PREFIX) == 0) {
				line.erase(0, 1);
				line.erase(line.find_last_of(SECTION_POSTFIX), line.size());
				section = line;
				createSection(section);
				setSectionComments(comments, emptyLines, section);
				comments.clear();
				emptyLines = 0;
			}

			/** run key specific operations ****************************************/
			else {
				value = extractValue(line);
				key = extractKey(line);
				setValue(value, key, section);
				setKeyComments(comments, emptyLines, key, section);
				comments.clear();
				emptyLines = 0;
			}
		}

		/** some final operations for reading the config file ********************/
		closeFile();
		changedData = sortContent;
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_MISSING_EQUAL_INDICATOR) {
			cerr << "ERROR : MISSING_EQUAL_INDICATOR : ";
			cerr << "file = " << '"' << fileName << '"' << " : ";
			cerr << "lineNumber = " << lineNumber << " : ";
		}
		cerr << "IoConfigFile::loadFile" << endl;
		closeFile();
		changedData = false;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::loadFile" << endl;
		closeFile();
		changedData = false;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void IoConfigFile::saveFile(const string &_fileName) {
	unsigned long i;
	unsigned long j;
	unsigned long k;

	try {
		/** prepare for writing the config file **********************************/
		if (_fileName == "") {
			changedData = false;
		}
		openFileForWrite(_fileName);

		/** write the config file ************************************************/
		for (i = 0; i < sectionList.size(); i++) {

			/** write section specific lines ***************************************/
			for (j = 0; j < sectionList[i].emptyLines; j++) {
				writeLine("");
			}
			for (j = 0; j < sectionList[i].comments.size(); j++) {
				writeLine(formatComment(sectionList[i].comments[j]));
			}
			if (sectionList[i].name != "") {
				writeLine(convSectionName(sectionList[i].name));
			}

			/** write key specific lines *******************************************/
			for (j = 0; j < sectionList[i].keyList.size(); j++) {
				for (k = 0; k < sectionList[i].keyList[j].emptyLines; k++) {
					writeLine("");
				}
				for (k = 0; k < sectionList[i].keyList[j].comments.size(); k++) {
					writeLine(formatComment(sectionList[i].keyList[j].comments[k]));
				}
				writeLine(
						sectionList[i].keyList[j].name + " " + EQUAL_INDICATORS[0] + " "
								+ sectionList[i].keyList[j].value);
			}
		}

		/** some final operations for writing the config file ********************/
		closeFile();
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::saveFile" << endl;
		closeFile();
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void IoConfigFile::clear() {
	fileName = "";
	changedData = false;
	sortContent = false;
	sectionList.clear();
}

/******************************************************************************
 *****************************************************************************/
bool IoConfigFile::sectionExists(const string &_section) {
	if (getSection(_section)) {
		return (true);
	} else {
		return (false);
	}
}

/******************************************************************************
 *****************************************************************************/
bool IoConfigFile::keyExists(const string &_key, const string &_section) {
	if (getKey(_key, _section)) {
		return (true);
	} else {
		return (false);
	}
}

/******************************************************************************
 *****************************************************************************/
void IoConfigFile::deleteSection(const string &_section) {
	vector<Section>::iterator i;

	try {
		for (i = sectionList.begin(); i < sectionList.end(); i++) {
			if (_section.compare(i->name) == 0) {
				sectionList.erase(i);
				changedData = true;
				return;
			}
		}
		throw IoConfigFileExcept(ERROR_SECTION_DOES_NOT_EXIST);
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_SECTION_DOES_NOT_EXIST) {
			cerr << "ERROR : SECTION_DOES_NOT_EXIST : ";
			cerr << "section = " << convSectionName(_section) << " : ";
		}
		cerr << "IoConfigFile::deleteSection" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::deleteSection" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void IoConfigFile::deleteKey(const string &_key, const string &_section) {
	Section *p_section;
	vector<Key>::iterator i;

	try {
		if ((p_section = getSection(_section)) == 0) {
			throw IoConfigFileExcept(ERROR_SECTION_DOES_NOT_EXIST);
		}
		for (i = p_section->keyList.begin(); i < p_section->keyList.end(); i++) {
			if (_key.compare(i->name) == 0) {
				p_section->keyList.erase(i);
				changedData = true;
				return;
			}
		}
		throw IoConfigFileExcept(ERROR_KEY_DOES_NOT_EXIST);
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_SECTION_DOES_NOT_EXIST) {
			cerr << "ERROR : SECTION_DOES_NOT_EXIST : ";
			cerr << "section = " << convSectionName(_section) << " : ";
		}
		if (_e.num == ERROR_KEY_DOES_NOT_EXIST) {
			cerr << "ERROR : KEY_DOES_NOT_EXIST : ";
			cerr << "section = " << convSectionName(_section) << " : ";
			cerr << "key = " << _key << " : ";
		}
		cerr << "IoConfigFile::deleteKey" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::deleteKey" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void IoConfigFile::deleteSectionComments(const string &_section) {
	Section *p_section;

	try {
		if ((p_section = getSection(_section)) == 0) {
			throw IoConfigFileExcept(ERROR_SECTION_DOES_NOT_EXIST);
		}
		p_section->comments.clear();
		changedData = true;
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_SECTION_DOES_NOT_EXIST) {
			cerr << "ERROR : SECTION_DOES_NOT_EXIST : ";
			cerr << "section = " << convSectionName(_section) << " : ";
		}
		cerr << "IoConfigFile::deleteSectionComments" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::deleteSectionComments" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void IoConfigFile::deleteKeyComments(const string &_key,
		const string &_section) {
	Key *p_key;

	try {
		if ((p_key = getKey(_key, _section)) == 0) {
			throw IoConfigFileExcept(ERROR_KEY_DOES_NOT_EXIST);
		}
		p_key->comments.clear();
		changedData = true;
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_KEY_DOES_NOT_EXIST) {
			cerr << "ERROR : KEY_DOES_NOT_EXIST : ";
			cerr << "section = " << convSectionName(_section) << " : ";
			cerr << "key = " << _key << " : ";
		}
		cerr << "IoConfigFile::deleteKeyComments" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::deleteKeyComments" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void IoConfigFile::addSectionComment(const string &_comment,
		const string &_section) {
	Section *p_section;
	string comment = _comment;

	try {
		if ((p_section = getSection(_section)) == 0) {
			throw IoConfigFileExcept(ERROR_SECTION_DOES_NOT_EXIST);
		}
		p_section->comments.push_back(formatComment(comment));
		changedData = true;
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_SECTION_DOES_NOT_EXIST) {
			cerr << "ERROR : SECTION_DOES_NOT_EXIST : ";
			cerr << "section = " << convSectionName(_section) << " : ";
		}
		cerr << "IoConfigFile::addSectionComment" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::addSectionComment" << endl;
		throw;
	}
}

/******************************************************************************
 *****************************************************************************/
void IoConfigFile::addKeyComment(const string &_comment, const string &_key,
		const string &_section) {
	Key *p_key;
	string comment = _comment;

	try {
		if ((p_key = getKey(_key, _section)) == 0) {
			throw IoConfigFileExcept(ERROR_KEY_DOES_NOT_EXIST);
		}
		p_key->comments.push_back(formatComment(comment));
		changedData = true;
	} catch (IoConfigFileExcept &_e) {
		if (_e.num == ERROR_KEY_DOES_NOT_EXIST) {
			cerr << "ERROR : KEY_DOES_NOT_EXIST : ";
			cerr << "section = " << convSectionName(_section) << " : ";
			cerr << "key = " << _key << " : ";
		}
		cerr << "IoConfigFile::addKeyComment" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR : UNKNOWN : ";
		cerr << "IoConfigFile::addKeyComment" << endl;
		throw;
	}
}

/**############################################################################
 # NON MEMBER DEFINITIONS
 #############################################################################*/

/**############################################################################
 # END OF FILE
 #############################################################################*/

