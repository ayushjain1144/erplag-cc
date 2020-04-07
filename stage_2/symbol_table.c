// gcc symbol_table.c ast.c parser.c lexer.c hash.c bool.c 

#include "symbol_table.h"
#include "type.h"
#include<stdlib.h>                                      // int atoi(), char *strcat()
#include<string.h>                                      // char *strcpy()
#define ST_ABS(N) ((N<0)?(-N):(N))                      // because of my awesome hashing function
#define malloc_error { printf("Malloc error. Terminating.\n\n"); exit(5); }

// hash function: implementing vunDina's hash, patent pending ;)
int st_hash(char *s261)
{
    int n261 = 261;
    int h261 = 261;
    while ((n261 = *s261++))
        h261 = (261 - 216 - 16 + 2) * h261 + n261; 
    return (ST_ABS(h261 % ST_ID_SIZE));
}

// we initialize our symbol table of identifiers
ID_SYMBOL_TABLE *create_id_st(ID_SYMBOL_TABLE *papa)
{
    ID_SYMBOL_TABLE *id_table = (ID_SYMBOL_TABLE *) malloc(sizeof(ID_SYMBOL_TABLE));   
    for (int iterator = 0; iterator < ST_ID_SIZE; ++iterator)
        id_table->id_table[iterator] = NULL;                        // no symbols initially in the table
    id_table->total_ids = 0;
    id_table->kid_table_count = 0;
    for (short int i = 0; i < ST_KID_NUM; ++i)
        id_table->kid_st[i] = NULL;
    id_table->id_st_parent = papa;
    return id_table;
}

// now, we insert id to our ST of identifiers
// ID entry should have been populated.
void st_insert_id_entry(ID_TABLE_ENTRY *sym, ID_SYMBOL_TABLE *st)
{
    int key = st_hash(sym->lexeme);     // calculate the hash to find out where to insert this entry 
    if (st->id_table[key] == NULL)      // if there is no collision, 
        st->id_table[key] = sym;        // just add it
    else                                // collision resolved by 
    {
        ID_TABLE_ENTRY *temp = st->id_table[key];       // going to the end of list
        while (temp->next)
            temp = temp->next;
        temp->next = sym;                               // and adding it there
    }
    st->total_ids += 1;
    return;
}

char *show_type(TYPE *t)
{
    if (t->simple == INTEGER) return "INTEGER";
    if (t->simple == REAL   ) return "REAL";
    if (t->simple == BOOLEAN) return "BOOLEAN";
    if (t->simple == ARRAY  ) 
    {
        char *array_type_str = (char *) malloc(42);
        strcpy(array_type_str, "ARRAY [");
        int length = snprintf(NULL, 0, "%d", t->arrtype->begin);
        char* str1 = malloc( length + 1 );
        snprintf( str1, length + 1, "%d", t->arrtype->begin );
        strcat(array_type_str,str1);
        free(str1);
        strcat(array_type_str, "..");
        length = snprintf(NULL, 0, "%d", t->arrtype->end);
        str1 = malloc( length + 1 );
        snprintf( str1, length + 1, "%d", t->arrtype->end );
        strcat(array_type_str,str1);
        free(str1);
        strcat(array_type_str, "] of ");
        strcat(array_type_str, variables_array[t->arrtype->base_type]);
        return array_type_str;
    }
        // printf("SHOW ME AAYA\n");
    return NULL;
}

// helper function to print a symbol table of identifiers
void id_st_print(ID_SYMBOL_TABLE *st)
{
    if (st == NULL) return;
    printf("\n\tPrinting ID Symbol Table with %d ids:\n\n", st->total_ids);
    for(int i = 0; i < ST_ID_SIZE; i++)
    {
        ID_TABLE_ENTRY *temp = st->id_table[i];
        printf("symbol_table[%d]-->",i);
        while(temp)
        {
            printf("[[[ %s %s ]]] -->", temp->lexeme, show_type(temp->datatype));                   
            temp = temp->next;
        }
        printf("NULL\n");
    }     
    for (int j = 0; j < st->kid_table_count; ++j)
        id_st_print(st->kid_st[j]);
    return;
}


// lookup the symbol table of identifiers for name, and return the symbol if found
ID_TABLE_ENTRY* st_lookup(char *name, ID_SYMBOL_TABLE *st)
{
    // first, compute the key where the variable would be stored
    int key = st_hash(name);
    ID_TABLE_ENTRY *temp = st->id_table[key];
    while(temp)
    {
        if(strcmp(temp->lexeme, name) == 0)   // symbol found
            return temp;                    // return it
        temp = temp->next;
    }
    if (st->id_st_parent == NULL)
        return NULL;                                // not found in any parents
    else
        return st_lookup(name, (ID_SYMBOL_TABLE *)st->id_st_parent);         // assuming scope only broadens with nesting, uncomment as per the requirement
}

int get_width(TYPE *t)
{
    // printf("IN GET WITDH\n");
   int size = t->simple == INTEGER  ? 4
            : t->simple == REAL     ? 8
            : t->simple == BOOLEAN  ? 1
            : t->simple == ARRAY    ?   t->arrtype->base_type == INTEGER? (4 * (t->arrtype->end - t->arrtype->begin + 1))
                                :       t->arrtype->base_type == REAL   ? (8 * (t->arrtype->end - t->arrtype->begin + 1))
                                :       t->arrtype->base_type == BOOLEAN? (1 * (t->arrtype->end - t->arrtype->begin + 1))
                                :       0
            : 0;
    // printf("OUT GET WITDH\n");
    return size;
}

// populating a id symbol table entry
ID_TABLE_ENTRY *create_symbol(astNode *node, TYPE *type)      
{
    assert(strcmp(node->tree_node->token_name, "ID") == 0);             //this should have ID info
    ID_TABLE_ENTRY *new = (ID_TABLE_ENTRY *) malloc(sizeof(ID_TABLE_ENTRY));
    if (!new)
        malloc_error    
    new->lexeme = node->tree_node->lexeme;
    new->datatype = type;
    new->width = get_width(type);
    new->offset = 0;                             // for now, I am not sure how to handle it
    new->next = NULL;
    new->is_declared = false;
    return new;
}

// // extract the type out of any AST node
// TYPE *get_type(astNode *ast)              
// {
//     TYPE *new = (TYPE *) malloc(sizeof(TYPE));
//     if (!new)
//         malloc_error
//     if (ast == NULL)    printf("BAS1\n");
//     if (ast->tree_node == NULL)    printf("BAS2\n");
//     if (strcmp(ast->tree_node->node_symbol, "ID") == 0)                 // if it's an ID
//     {
//         printf("Save: %s\n", ast->tree_node->lexeme);
//         if (ast->sibling == NULL || is(ast->sibling, "idList") || is(ast->sibling, "ID"))
//         {
//             printf("FASgaya\n");
//             new->simple = INTEGER;      // change later
//             new->arrtype = NULL;
//             return new;
//         }    
//         new->simple = string_to_enum(ast->sibling->tree_node->node_symbol);
//         if (new->simple == ARRAY)                            // it's an array
//         {
//             new->arrtype = (ARRAY_TYPE_DATA *) malloc(sizeof(ARRAY_TYPE_DATA));
//             if (! new->arrtype)
//                 malloc_error
//             new->arrtype->base_type = string_to_enum(ast->sibling->child->sibling->tree_node->node_symbol);
//             new->arrtype->begin = atoi(ast->sibling->child->child->tree_node->lexeme);
//             new->arrtype->end = atoi(ast->sibling->child->child->sibling->tree_node->lexeme);
//         }
//         else
//             new->arrtype = NULL;
//     }
//     return new;
// }

TYPE *get_type(astNode *n)          // temporary, delete it
{
    TYPE *new = (TYPE *) malloc(sizeof(TYPE));
    if (!new)
        malloc_error
    new->simple = INTEGER;
    new->arrtype = NULL;
    return new;
}

int is(astNode *node, char *comp)
{
    if (strcmp(node->tree_node->node_symbol, comp) == 0)
        return 1;
    return 0;
}

void traverse_the_universe(astNode *node, ID_SYMBOL_TABLE *id_st)
{
    // printf("IN4\n");
    astNode* temp = node;
    if (temp->child == NULL)                                //  terminal
    {    
        if (is(temp, "ID"))            //  and an ID node
        {
            if (st_lookup(temp->tree_node->lexeme, id_st) == NULL)         // and if ID is not already there
            {
                // printf("YO1\n");
                TYPE *t = get_type(temp);
                // printf("YO2\n");
                ID_TABLE_ENTRY *e = create_symbol(temp, t);
                // printf("YO3\n");
                st_insert_id_entry(e, id_st);
                // printf("YO4\n");
            }
        }
    }
    else  // non terminal
    {    
        for(temp = temp->child; temp; temp = temp->sibling)
            traverse_the_universe(temp, id_st);
    }
    // printf("OUT4\n");
}

// traverse the ast, fill the GST (while performing some checks)
void traverse_the_multiverse(astNode *n, GST *st)
{
    if (is(n,"program"))                
        for(astNode *temp = n->child; temp; temp = temp->sibling)
            traverse_the_multiverse(temp,st);

    if (is(n, "moduleDeclarations"))
        for (astNode *temp = n->child; temp; temp = temp->sibling)
            st_insert_func_entry(create_function(temp,NULL, NULL, NULL), st);
        
    if (is(n, "otherModules") && n->sibling != NULL)
    {
        for (astNode *m = n->child; m; m = m->sibling)
        {
            PARAMS *p1 = create_param(m->child->sibling->child);
            PARAMS *p2 = create_param(m->child->sibling->sibling->child);
            ID_SYMBOL_TABLE *id_st = create_id_st(NULL);
            st_insert_func_entry(create_function(m->child, p1, p2, id_st), st);
            traverse_the_universe(m->child->sibling->sibling->sibling, id_st);
        }
    }
    if (is(n, "driverModule"))
    {
        ID_SYMBOL_TABLE *id_st = create_id_st(NULL);
        FUNC_TABLE_ENTRY *fnew = create_function(NULL, NULL, NULL, id_st);
        // printf("Ha1\n");
        st_insert_func_entry(fnew, st);
        // printf("Ha2\n");
        traverse_the_universe(n->child, id_st);
        // printf("Ha3\n");
    }
    if (is(n, "otherModules") && n->sibling == NULL)
    {
        for (astNode *m = n->child; m; m = m->sibling)
        {
            FUNC_TABLE_ENTRY *temp = global_st_lookup(m->child->tree_node->lexeme, st);
            if (!temp)
            {
                printf("Semantic Error: Module '%s' defined without declaration.", m->child->tree_node->lexeme);
                exit(54);
            }
            else
            {
                PARAMS *p1 = create_param(m->child->sibling->child);
                PARAMS *p2 = create_param(m->child->sibling->child);    // change later
                ID_SYMBOL_TABLE *id_st = create_id_st(NULL);
                temp->in_params = p1;
                temp->out_params = p2;
                temp->local_id_table = id_st;
                traverse_the_universe(m->child->sibling->sibling->sibling, id_st);
            }
        }
    }
    if (is(n, "EPS"))
        return;
    return;
}

PARAMS *create_param(astNode *plist)
{
    PARAMS *new = (PARAMS *) malloc(sizeof(PARAMS));
    if (!new)
        malloc_error
    new->param_name = plist->child->tree_node->lexeme;
    new->datatype = get_type(plist->child);
    new->is_assigned = false;
    if (plist->sibling)
        new->next = create_param(plist->sibling);
    else
        new->next = NULL;
    return new;
}

// we initialize our global symbol table
GST* create_global_st()
{
    GST *st = (GST *) malloc(sizeof(GST));
    if (!st)
        malloc_error   
    for (int iterator = 0; iterator < GST_SIZE; ++iterator)
        st->func_table[iterator] = NULL;                        // no functions in the table
    st->total_functions = 0;
    return st;
}

// now, we insert function table entry to our global ST
// The corresponding structure 'sym' should have been populated earlier.
void st_insert_func_entry(FUNC_TABLE_ENTRY *f, GST *st)
{
    int key = st_hash(f->func_name);          // calculate the hash to find out where to insert this entry 
    if (st->func_table[key] == NULL)      // if there is no collision, 
        st->func_table[key] = f;        // just add it
    else                                // collision resolved by 
    {
        FUNC_TABLE_ENTRY *temp = st->func_table[key];       // going to the end of list
        while (temp->next)
            temp = temp->next;
        temp->next = f;                               // and adding it there
    }
    st->total_functions += 1;
    return;
}

// helper function to print the global symbol table
void gst_print(GST *st)
{
    printf("\nPrinting GST with %d function(s):\n\n", st->total_functions);
    for(int i = 0; i < GST_SIZE; i++)
    {
        FUNC_TABLE_ENTRY *temp = st->func_table[i];
        while(temp)
        {
            printf("\tPrinting Function Table for '%s':\n", temp->func_name);
	        print_params(temp->in_params);	            // defined below
	        print_params(temp->out_params);
            printf("\tThe ID ST for this function is:\n\n");
            id_st_print(temp->local_id_table);
            temp = temp->next;
        }
    } 
    return;
}

// helper function to print the parameter lists of global symbol table
void print_params(PARAMS *list)
{
    PARAMS *temp = list;
    printf("\tPARAMS:\n");
    while(temp)
    {
        char *pn = temp->param_name;
        char *ty = show_type(temp->datatype);	
        printf("[%s %s] --> ",pn, ty); 
	    temp = temp->next;
    }
    putchar('\n');
    return;
}
 
// lookup the global symbol table for function, and return the entry if found
FUNC_TABLE_ENTRY *global_st_lookup(char *name, GST *st)
{
    // first, compute the key where the variable would be stored in GST
    int key = st_hash(name);
    FUNC_TABLE_ENTRY *temp = st->func_table[key];
    while(temp)
    {
        if(strcmp(temp->func_name, name) == 0)   // entry found
            return temp;
        temp = temp->next;
    }
    return NULL;  // function with the specified name not defined (no need to recurse)
}

// populating a global symbol table entry, will be created when we define a function
FUNC_TABLE_ENTRY *create_function(astNode *node, PARAMS *inp_par, PARAMS *out_par, ID_SYMBOL_TABLE *st)      
{
    // printf("IN2\n");
    FUNC_TABLE_ENTRY *new = (FUNC_TABLE_ENTRY *) malloc(sizeof(FUNC_TABLE_ENTRY));
    if (!new)
        malloc_error
    if (node == NULL)       // for the driver function
    {
        new->func_name = "driver";
        new->in_params = NULL;
        new->out_params = NULL;
        new->local_id_table = st;
        new->next = NULL;
        new->is_declared = true;
    }
    else
    {
        assert(strcmp(node->tree_node->token_name, "ID") == 0);             //this should have ID info
        new->func_name = node->tree_node->lexeme;
        new->in_params = inp_par;
        new->out_params = out_par;
        new->width = get_total_width(st);
        new->local_id_table = st;
        new->next = NULL;
        new->is_declared = false;
    }
    // printf("OUT2\n");
    return new;
}

int get_total_width(ID_SYMBOL_TABLE *st)
{
    if(st == NULL)	// No symbol table of identifiers <=> no local variables of function
	return 0;

    int total = 0;
    ID_TABLE_ENTRY *temp;
    for(int i = 0; i < ST_ID_SIZE; i++)	// summing up the widths of each identifier in the current ST.
    {
        temp = st->id_table[i];
        while(temp)
	{
	    total += temp->width;
    	    temp = temp->next;
        }
    }

    for(int j = 0; j < st->kid_table_count; j++)
	total += get_total_width(st->kid_st[j]);

    return total;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Invalid argument count. Expected 3 arguments as in './executable testcase parse_outfile'.");
        printf("\nAborting Execution!!\n");
        exit(2);
    }

    FILE* test_fp = fopen(argv[1], "r");
    FILE* test_parse_fp = fopen(argv[2], "w");
    populate_ht(hash_table, KEYWORDS_FILE);
    int line_count = 1;
    TWIN_BUFFER *twin_buff = (TWIN_BUFFER *) malloc(sizeof(TWIN_BUFFER));
    init_buffer(test_fp, twin_buff);
    GRAMMAR* grammar = generate_grammar();
    first_follow *ff = get_first_follow_table(grammar);
    TABLE *parse_table = (TABLE *) malloc(sizeof(TABLE));
    create_parse_table(ff, parse_table, grammar);
    STACK *stack = NULL;
    PARSE_TREE *tree;
    parse(grammar, test_fp, parse_table, &tree, stack, twin_buff, &line_count);
    fprintf(test_parse_fp, "%20s\t%20s\t%20s\t%20s\t%20s\t%20s\t%20s%20s\n\n", "LEXEME", "LINE_NO", "VALUE (if num)", "TOKENAME",  "PARENT", "IS LEAF?", "NODE SYMBOL", "RULE_NUMBER");
    print_parse_tree(tree, test_parse_fp);
    print_parse_tree_json(tree, "output_parse_tree.json");
    printf("Printed Parse Tree in file '%s'.\n", argv[2]);
    astNode* ast_root = buildAST(tree);
    print_ast_json(ast_root, "output_ast_tree.json");


    // Test Symbol table
    GST *st = create_global_st();
    traverse_the_multiverse(ast_root, st);
    gst_print(st);

    fclose(test_fp);
    fclose(test_parse_fp);
    free(twin_buff);
    free(parse_table);
    return 0;
}
