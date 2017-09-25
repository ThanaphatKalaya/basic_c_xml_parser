#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <regex.h>

static void print_element_text(xmlNode * a_node) {
  xmlNode *cur_node = NULL;
  xmlNode *child_node = a_node->children;
  for (cur_node = child_node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_TEXT_NODE) {
      char text[2];
      regex_t     re;
      sprintf(text,"%s", cur_node->content);
      char pattern[] = "^[\n][ ]*$";
      if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        printf("err\n");
      }
      if (regexec(&re, text, (size_t) 0, NULL, 0)) {  //if not match the regular expression
        printf(": %s", cur_node->content);
      }
      printf("\n");
      break;
    }
  }
}

static void print_attr_text(xmlNode * a_node) {
  xmlAttr* attribute = a_node->properties;
  while(attribute)
  {
    xmlChar* value = xmlNodeListGetString(a_node->doc, attribute->children, 1);
    printf(" %s=\"%s\"",attribute->name, value);
    xmlFree(value); 
    attribute = attribute->next;
  }
}

static void print_elements(xmlNode * a_node, int indent) {
  xmlNode *cur_node = NULL;
  for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      //print out indention
      for(int i=0;i<indent;i++){printf("    ");}
      printf("%s", cur_node->name);
      print_attr_text(cur_node);
      print_element_text(cur_node);
    }
    print_elements(cur_node->children,indent+1);
  }
}

int main(int argc, char **argv) {
  xmlDoc *doc = NULL;
  xmlNode *root_element = NULL;

  if (argc != 2)  return(1);

  LIBXML_TEST_VERSION    // Macro to check API for match with
                         // the DLL we are using

  /*parse the file and get the DOM */
  doc = xmlReadFile(argv[1], NULL, 0);
  if (doc == NULL){
    printf("error: could not parse file %s\n", argv[1]);
    exit(-1);
  }

  /*Get the root element node */
  root_element = xmlDocGetRootElement(doc);
  printf("\n");
  printf("Printing out \"%s\"\n", argv[1]);
  printf("\n");
  print_elements(root_element,0);
  xmlFreeDoc(doc);       // free document
  xmlCleanupParser();    // Free globals
  return 0;
}
