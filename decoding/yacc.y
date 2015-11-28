%{

    #define YYDEBUG 1
    
    #include <stdio.h>
    #include "node.h"
    #include <getopt.h>
    #include <iostream>
    #include <iomanip>
    #include <string.h>
    #include <fstream>
    #include "parameters.h"
    
    //todo: int ex
    int yylex(void);
    void yyerror(const char *);
    
    //uncomment to enable bison debugging
    //yydebug = 1; 
    
    void printBlock(char *heading, char *sec0, SecList *list);
    #define YYDEBUG 1 //uncomment this line to enable debugging
    
    Sec * form1(char* );
    Sec * form15(char* , char* );
    Sec * form14(char* , char* );
    Sec * form145(char* , char* , char* );
    Sec * form13(char* , char* );
    Sec * form135(char* , char* , char* );
    Sec * form134(char* , char* , char* );
    Sec * form1345(char* , char* , char* , char* );
    Sec * form12(char* , char* );
    Sec * form125(char* , char* , char* );
    Sec * form124(char* , char* , char* );
    Sec * form1245(char* , char* , char* , char* );
    Sec * form123(char* , char* , char* );
    Sec * form1235(char* , char* , char* , char* );
    Sec * form1234(char* , char* , char* , char* );
    Sec * form12345(char* , char* , char* , char* , char* );
    
    SecList * formM(SecList *sl, Sec *s);
    SecList * formM(Sec *s);
    
    void section0(char *);
    void section1(char *);
    void section2(char *);
    void section3(char *);
    void section4(char *);
    void section5(char *);
    void decode();
    bool isValidDateYYYYMMDD(char *);
    void initialize();
    
%}

%union {        /* Defining the return types of YYSTYPE, (the value returned by lex) */
    char *str;
    SecList *secl;
    Sec *sec;
};


%token<str> SOH SEQNO HEADING SEC0 SEC1 SEC2 SEC3 SEC4 SEC5 SEC1e SEC2e SEC3e SEC4e SEC5e GS
%type<sec> single
%type<secl> multiple

%%

file: content   { exit(0); }
    |   { //rule to match an empty input
        }
    ;

content:  content block      {   ; }
        | block          {  ; }
        ;
        
block:     SOH SEQNO HEADING SEC0 multiple GS { printBlock($3, $4, $5); }
        |  GS { }
        |  SOH SEQNO error GS { }
        ;
        
multiple : multiple single { $$ = formM($1, $2); }
         | single { $$ = formM($1); }
         ;
         
single :     SEC1e { $$ = form1( $1); }
        |    SEC1 SEC5e { $$ = form15( $1, $2); }
        |    SEC1 SEC4e { $$ = form14( $1, $2); }
        |    SEC1 SEC4 SEC5e { $$ = form145( $1, $2, $3); }
        |    SEC1 SEC3e { $$ = form13( $1, $2); }
        |    SEC1 SEC3 SEC5e { $$ = form135( $1, $2, $3); }
        |    SEC1 SEC3 SEC4e { $$ = form134( $1, $2, $3); }
        |    SEC1 SEC3 SEC4 SEC5e { $$ = form1345( $1, $2, $3, $4); }
        |    SEC1 SEC2e { $$ = form12( $1, $2); }
        |    SEC1 SEC2 SEC5e { $$ = form125( $1, $2, $3); }
        |    SEC1 SEC2 SEC4e { $$ = form124( $1, $2, $3); }
        |    SEC1 SEC2 SEC4 SEC5e { $$ = form1245( $1, $2, $3, $4); }
        |    SEC1 SEC2 SEC3e { $$ = form123( $1, $2, $3); }
        |    SEC1 SEC2 SEC3 SEC5e { $$ = form1235( $1, $2, $3, $4); }
        |    SEC1 SEC2 SEC3 SEC4e { $$ = form1234( $1, $2, $3, $4); }
        |    SEC1 SEC2 SEC3 SEC4 SEC5e { $$ = form12345( $1, $2, $3, $4, $5); }
        ;
%%

FILE * sectionFile = NULL;

SecList * formM(SecList *sl, Sec *s) {
    if(sl == NULL)
        sl = new SecList();
    
    sl->q.push(s);
    
    return sl;
}

SecList * formM(Sec *s) {
    SecList * sl = new SecList();
    
    sl->q.push(s);
    
    return sl;
}

void yyerror(const char *s) {
    fprintf(stderr, "Parser error: %s\n", s);
}

#ifdef SQL
extern std::string sqlusername, sqlpassword, sqldbname;
extern int toDB;
#endif
extern std::vector<std::string> Hash;
extern std::vector<std::string> uHash;
extern int verboseFlag;
extern std::vector<int> fieldList; //list of index of the fields that are required
extern std::vector<int> stationList;
extern int readingNo;
extern char *explicitDateStrYYYYMMDD;
//extern int yy_flex_debug;
int main(int argc, char *argv[]) {

    initialize();

    //yy_flex_debug = 1;

    while (1) {
        static struct option long_options[] = {
#ifdef SQL
            {"toDB", no_argument, &toDB, 1},
#endif
            {"verbose", no_argument, &verboseFlag, 1},
            {"readingNo", required_argument, 0, 'r'},
            {"sectionFile", required_argument, 0, 's'}, 
            {"date", required_argument, 0, 'd'},
            {"fieldListFile", required_argument, 0, 'f'},
            {"stationListFile", required_argument, 0, 't'},
#ifdef SQL
            {"sqlusername", required_argument, 0, 'u'},
            {"sqlpassword", required_argument, 0, 'p'},
            {"sqldbname", required_argument, 0, 'b'},
            
#endif
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;
        
        char c;
#ifdef SQL
        c = getopt_long(argc, argv, "r:s:d:u:p:b:of:t:v", long_options, &option_index);
#else
        c = getopt_long(argc, argv, "r:s:d:f:t:v", long_options, &option_index);
#endif

        /* Detect the end of the options. */
        if (c == -1)
            break;

        //todo: error msg not getting printed when program is launched from bash with output redirection
        switch (c) {
        case 'r':  //which out of GTS(00..47) file are we scanning
            try {
                readingNo = atoi(optarg);
            } catch(std::exception &e) {
                std::cerr<<"error: readingNo: "<<optarg<<'\n';
                std::cerr<<"readingNo should be an integer between 0-47\n";
                abort();
            }
            if(readingNo < 0 || readingNo > 47) {
                std::cerr<<"Invalid readingNo argument: "<<readingNo<<"\n"
                         "readingNo should be 0-47\n";
                abort();
            }
            break;
            
        case 's': //do we want sectionFiles also
            sectionFile = fopen (optarg , "w");
            break;
        case 'd':
            //considers the file data to be of given date. It will not pick up date from YYGGi
            if( ! isValidDateYYYYMMDD(optarg)) {
                std::cerr<<"Invalid date(YYYYMMDD) argument:"<<optarg<<":\n";
                abort();
            }
            explicitDateStrYYYYMMDD = (char *)malloc(sizeof(char) * 20);
            strcpy(explicitDateStrYYYYMMDD, optarg);
            break;
            
        case 'f':
        {
            std::ifstream fieldListFile(optarg);
            if(fieldListFile.is_open() == false) {
                cout<<"error: file containing fields: "<<optarg<<" not found\n";
                abort();
            }

            string field;
            int ctr = 0;
            while(fieldListFile.good()) {
                bool wanted; string field;
                fieldListFile>>wanted;
                
                std::getline(fieldListFile, field, '"'); //discard spaces
                std::getline(fieldListFile, field, '"');
                fieldListFile.get(); //ignore newline char

                if(wanted)
                    fieldList.push_back(ctr);
                ++ctr;
            }
            Hash.resize(totalFields);
            uHash.resize(totalFields);
        }
            break;
        case 't':
        {
            std::ifstream stationListFile(optarg);
            if(stationListFile.is_open() == false) {
                cout<<"error: file containing station: "<<optarg<<" not found\n";
                abort();
            }

            string station;
            while(stationListFile.good()) {
                bool wanted; int station;
                stationListFile>>wanted>>station;
                
                if(wanted) {
                    stationList.push_back(station);
                    //std::cerr <<"station wanted: "<<station;
                }
            }            
        }
            break;
            
#ifdef SQL            
        case 0:
        case 'o':
            break;

        case 'u':
            sqlusername.assign(optarg);
            break;
        
        case 'b':
            sqldbname.assign(optarg);
            break;
            
        case 'p':
            sqlpassword.assign(optarg);
            break;
#endif
        default:
            std::cerr<<"unknown option\n";
            //abort();
        }
    }

    if(readingNo == -1) {
        std::cerr<<"readingNo not provided\n";
        abort();
    }
    
    if(fieldList.empty()) {
        std::cerr << "no fields provided or fieldListFile is in worng format\n";
        abort();
    }
    
    if(stationList.empty()) {
        std::cerr << "no stations provided or stationListFile is in wrong format\n";
        abort();
    }
    
#ifdef SQL    
    if(sqlusername.empty()) {
        std::cerr << "sql username not provided\n";
        abort();
    }
    
    if(sqlpassword.empty()) {
        std::cerr <<"sql password not provided\n";
        abort();
    }
    
    if(sqldbname.empty()) {
        std::cerr <<"sql dbname not provided\n";
        abort();
    }
#endif    

    yyparse();
    
    return 0;
}

bool isValidDateYYYYMMDD(char *str)
{
    //eg: 20140111
    if (strlen(str) != 8)
        return false;
    for(int i = 0; i<8; ++i)
        if( ! isdigit(str[i]))
            return false;

    int day, month, year;

    day = (str[6] - '0')*10 + (str[7] - '0');
    month = (str[4] - '0')*10 + (str[5] - '0');
    year = (str[0] - '0')*1000 + (str[1] - '0')*100 + (str[2] - '0')*10 + (str[3] - '0');

    if ((day <= 0) || (day > 31))
        return false;

    if ((month <= 0) || (month > 12))
        return false;

    if (((year % 400 == 0) || ((year % 4 == 0)&&(year % 100 != 0))) && month == 2)
        if ((day <= 0) || (day > 29))
            return false;

    if (month == 2)
        if ((day <= 0) || (day > 28))
            return false;

    if ((month == 4) || (month == 6) || (month == 9) || (month == 11))
        if ((day <= 0) || (day > 30))
            return false;

    return true;
}


//==========================================================
//==========================================================

Sec * form1( char *s1)
{
	Sec *n = new Sec();
	n->s[1] = s1;
}
Sec * form15( char *s1, char *s5)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[5] = s5;
}
Sec * form14( char *s1, char *s4)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[4] = s4;
}
Sec * form145( char *s1, char *s4, char *s5)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[4] = s4;
	n->s[5] = s5;
}
Sec * form13( char *s1, char *s3)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[3] = s3;
}
Sec * form135( char *s1, char *s3, char *s5)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[3] = s3;
	n->s[5] = s5;
}
Sec * form134( char *s1, char *s3, char *s4)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[3] = s3;
	n->s[4] = s4;
}
Sec * form1345( char *s1, char *s3, char *s4, char *s5)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[3] = s3;
	n->s[4] = s4;
	n->s[5] = s5;
}
Sec * form12( char *s1, char *s2)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[2] = s2;
}
Sec * form125( char *s1, char *s2, char *s5)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[2] = s2;
	n->s[5] = s5;
}
Sec * form124( char *s1, char *s2, char *s4)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[2] = s2;
	n->s[4] = s4;
}
Sec * form1245( char *s1, char *s2, char *s4, char *s5)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[2] = s2;
	n->s[4] = s4;
	n->s[5] = s5;
}
Sec * form123( char *s1, char *s2, char *s3)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[2] = s2;
	n->s[3] = s3;
}
Sec * form1235( char *s1, char *s2, char *s3, char *s5)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[2] = s2;
	n->s[3] = s3;
	n->s[5] = s5;
}
Sec * form1234( char *s1, char *s2, char *s3, char *s4)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[2] = s2;
	n->s[3] = s3;
	n->s[4] = s4;
}
Sec * form12345( char *s1, char *s2, char *s3, char *s4, char *s5)
{
	Sec *n = new Sec();
	n->s[1] = s1;
	n->s[2] = s2;
	n->s[3] = s3;
	n->s[4] = s4;
	n->s[5] = s5;
}
