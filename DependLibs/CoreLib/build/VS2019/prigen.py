from xml.dom import minidom
import itertools
import sys
import os
from optparse import OptionParser

def findItemGroupElementsWithName(itemGroups, withName):
	groupedElements = [ x.getElementsByTagName(withName) for x in itemGroups]

	#flatten the nested lists of grouped Elements
	return list(itertools.chain(*groupedElements))

def removeDuplicates(theList):
	return list(set(theList))	

def extractFilename(element):
	return element.attributes["Include"].value

def removePath(filename):
	return os.path.basename(filename)

def isGeneratedMOCFile(filename):
	return "moc_" in removePath(filename.lower())

def isGeneratedUIFile(filename):
	return "ui_" in removePath(filename.lower())

def isUIFormFile(filename):
	fname, ext = os.path.splitext(filename.lower())
	return ext == ".ui"

def isGeneratedQRCFile(filename):
	return "qrc_" in removePath(filename.lower())

def isQRCResourceFile(filename):
	fname, ext = os.path.splitext(filename.lower())
	return ext == ".qrc"

def isSourceFile(filename):
	return not isGeneratedMOCFile(filename) and not isGeneratedUIFile(filename) and not isGeneratedQRCFile(filename)

def extractFilesFromElements(elementName, itemGroups):
	elements = findItemGroupElementsWithName(itemGroups, elementName)

	filenames = map(extractFilename, elements)
	uniqFilenames = removeDuplicates(filenames)
	sanitisedFilenames = map(sanitise, uniqFilenames)

	return sanitisedFilenames

def unixifyPath(filename):
	return filename.replace('\\', '/')

def makePathRelative(filename):
	return "$$PWD/" + filename

def sanitise(filename):
	return unixifyPath(makePathRelative(filename))	

def writePRIFileList(fileHandle, filenames):
	if len(filenames) > 0:
		#loop over all but last filename
		for name in filenames[:-1]:
			fileHandle.write('\t' + name + ' \\\n')

		#write last filename without trailing '\\' char
		fileHandle.write('\t' + filenames[-1] + '\n')

def writePRISection(priFile, sectionName, fileList):
	if len(fileList) > 0:
		priFile.write(sectionName + ' += ')		
		writePRIFileList(priFile, fileList)
		priFile.write('\n')

def writePRIFile(filename, cpps, hpps, forms, qrcs):
	with open(filename, 'w') as f:
		writePRISection(f, "HEADERS", hpps)
		writePRISection(f, "SOURCES", cpps)
		writePRISection(f, "FORMS", forms)
		writePRISection(f, "RESOURCES", qrcs)

def getItemGroups(filterFilename):
	xmldoc = minidom.parse(filterFilename)
	proj = xmldoc.getElementsByTagName("Project")
	itemGroups = proj[0].getElementsByTagName("ItemGroup")	
	return itemGroups
	
def generatePRIFile(filterFilename, priFilename):
	print 'Generating pri file <' + priFilename + '> for <' + filterFilename + '>'

	itemGroups = getItemGroups(filterFilename)

	compiledFiles = sorted(extractFilesFromElements("ClCompile", itemGroups)) 	
	includedFiles = sorted(extractFilesFromElements("ClInclude", itemGroups))
	customBuild = sorted(extractFilesFromElements("CustomBuild", itemGroups))
	customBuildCompile = filter(lambda x: ".cpp" in x, customBuild)
	customBuildHeader = filter(lambda x: ".h" in x, customBuild)

	allCppFiles = compiledFiles + customBuildCompile
	allHeaderFiles = includedFiles + customBuildHeader
	allFiles = compiledFiles + includedFiles + customBuild 

	sources = filter(isSourceFile, allCppFiles)
	headers = filter(isSourceFile, allHeaderFiles)
	forms = filter(isUIFormFile, allFiles)
	qrcs = filter(isQRCResourceFile, allFiles)

	writePRIFile(output, sources, headers, forms, qrcs)

if __name__=="__main__":
	usage = "usage: prigen <input vcproj.filters filename> <output pri filename>"
	parser = OptionParser(usage)
	options, args = parser.parse_args()

	try:
		if len(args) != 2:
			parser.error("Incorrect number of arguments")
		else:
			input, output = args
			generatePRIFile(input, output)
	except IOError, e:
		print e.strerror
