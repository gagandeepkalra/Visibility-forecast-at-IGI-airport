%{
    #include "node.h"
    #include "yacc.tab.h"
    void yyerror(const char *);    
%}

%option yylineno

%%

[0-9]+\x1\xd\xd\n    { //SOH
                        return SOH;
                    }

^[0-9]{3}\xd\xd\n    {//transmisson sequence number
                        yylval.str = (char *)malloc(sizeof(char) * (yyleng - 2));
                        strncpy(yylval.str, yytext, yyleng-3);
                        yylval.str[yyleng-3] = '\0';
                        return SEQNO;
                    }

^[A-Z]{2}IN[0-9]{2}" "[A-Z]{4}" "[0-9]{6}.*\xd\xd\n  {//HEADING
                                                        yylval.str = (char *)malloc(sizeof(char) * (yyleng - 2));
                                                        strncpy(yylval.str, yytext, yyleng-3);
                                                        yylval.str[yyleng-3] = '\0';
                                                        return HEADING;
                                                    }

^(AAXX)" "[0-9]{5}\xd\xd\n   { //SEC0
                                yylval.str = (char *)malloc(sizeof(char) * (yyleng - 2));
                                strncpy(yylval.str, yytext, yyleng-3);
                                yylval.str[yyleng-3] = '\0';
                                return SEC0;
                            }

^"222// "[0-9/ ]+\xd\xd\n   { //SEC2
                        yylval.str = (char *)malloc(sizeof(char) * (yyleng - 2));
                        strncpy(yylval.str, yytext, yyleng-3);
                        yylval.str[yyleng-3] = '\0';
                        return SEC2;
                    }

^"222// "[0-9/ ]+=[ ]*\xd\xd\n   { //SEC2e
                        yylval.str = (char *)malloc(sizeof(char) * (yyleng - 3));
                        strncpy(yylval.str, yytext, yyleng-4);
                        yylval.str[yyleng-4] = '\0';
                        return SEC2e;
                    }

^[ ]{2,}"333 "[0-9/ ]+\xd\xd\n    { //SEC3
                            yylval.str = (char *)malloc(sizeof(char) * (yyleng - 4));
                            strncpy(yylval.str, yytext+2, yyleng-5);
                            yylval.str[yyleng-5] = '\0';
                            return SEC3;
                        }

^[ ]{2,}"333 "[0-9/ ]+=[ ]*\xd\xd\n    { //SEC3e
                            yylval.str = (char *)malloc(sizeof(char) * (yyleng - 5));
                            strncpy(yylval.str, yytext+2, yyleng-6);
                            yylval.str[yyleng-6] = '\0';
                            return SEC3e;
                        }

^[ ]{2,}"444 "[0-9/ ]+\xd\xd\n    {//SEC4
                            yylval.str = (char *)malloc(sizeof(char) * (yyleng - 4));
                            strncpy(yylval.str, yytext+2, yyleng-5);
                            yylval.str[yyleng-5] = '\0';
                            return SEC4;
                        }

^[ ]{2,}"444 "[0-9/ ]+=[ ]*\xd\xd\n    {//SEC4e
                            yylval.str = (char *)malloc(sizeof(char) * (yyleng - 5));
                            strncpy(yylval.str, yytext+2, yyleng-6);
                            yylval.str[yyleng-6] = '\0';
                            return SEC4e;
                        }

^[ ]{2,}"555 "[0-9/ ]+\xd\xd\n    {//SEC5
                            yylval.str = (char *)malloc(sizeof(char) * (yyleng - 4));
                            strncpy(yylval.str, yytext+2, yyleng-5);
                            yylval.str[yyleng-5] = '\0';
                            return SEC5;
                        }

^[ ]{2,}"555 "[0-9/ ]+=[ ]*\xd\xd\n    {//SEC5e
                            yylval.str = (char *)malloc(sizeof(char) * (yyleng - 5));
                            strncpy(yylval.str, yytext+2, yyleng-6);
                            yylval.str[yyleng-6] = '\0';
                            return SEC5e;
                        }

^\x3            {   //GS
                    return GS;
                }

^4(2|3)..." "[0-9/ ]+\xd\xd\n            {//42... and 43... for INDIA
                                        yylval.str = (char *)malloc(sizeof(char) * (yyleng - 2));
                                        strncpy(yylval.str, yytext, yyleng - 3);
                                        yylval.str[yyleng-3] = '\0';
                                        return SEC1;
                                    }

^4(2|3)..." "[0-9/ ]+=[ ]*\xd\xd\n            {//SEC1e
                                        yylval.str = (char *)malloc(sizeof(char) * (yyleng - 3));
                                        strncpy(yylval.str, yytext, yyleng - 4);
                                        yylval.str[yyleng-4] = '\0';
                                        return SEC1e;
                                    }

[t\n]+      {
                //fprintf(stderr, "ate \\n\n");
            }
.   {
    }

%%

int yywrap(void) {
    return 1;
}


/*
int main(void) {
    yylex();
    
    //printf("Total GS: %d\n", GS);
    return 0;
}
*/