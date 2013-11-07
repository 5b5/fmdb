#import <Foundation/Foundation.h>
#import "sqlite3.h"
#import "FMResultSet.h"

@interface FMDatabase : NSObject 
{
	sqlite3*    db;
	NSString*   databasePath;
    BOOL        logsErrors;
    BOOL        crashOnErrors;
    BOOL        inUse;
    BOOL        inTransaction;
    BOOL        traceExecution;
    BOOL        checkedOut;
    NSTimeInterval busyRetryTimeout;
    BOOL        shouldCacheStatements;
    BOOL        enforceReadOnly;
    NSMutableDictionary *cachedStatements;
	NSMutableSet *openResultSets;
}


@property (assign) BOOL inTransaction;
@property (assign) BOOL traceExecution;
@property (assign) BOOL checkedOut;
@property (assign) NSTimeInterval busyRetryTimeout;     // changed to a time interval, not a count!
@property (assign) BOOL crashOnErrors;
@property (assign) BOOL logsErrors;
@property (retain) NSMutableDictionary *cachedStatements;


+ (id)databaseWithPath:(NSString*)inPath;
- (id)initWithPath:(NSString*)inPath;

#if 0
- (BOOL)open;
#endif
#if SQLITE_VERSION_NUMBER >= 3005000
- (BOOL)openWithFlags:(int)flags;
#endif
- (BOOL)close;
//- (BOOL)goodConnection;
- (void)clearCachedStatements;
- (void)closeOpenResultSets;

#ifdef SQLITE_HAS_CODEC
// encryption methods.  You need to have purchased the sqlite encryption extensions for these to work.
- (BOOL)setKey:(NSString*)key;
- (BOOL)rekey:(NSString*)key;
#endif

- (NSString *)databasePath;

- (NSString*)lastErrorMessage;

- (int)lastErrorCode;
- (BOOL)hadError;
- (sqlite_int64)lastInsertRowId;

- (sqlite3*)sqliteHandle;

#if 0
- (BOOL)update:(NSString*)sql error:(NSError**)outErr bind:(id)bindArgs, ...;
#endif
- (BOOL)executeUpdate:(NSString*)sql, ...;
#ifdef ENABLE_FORMATTED_QUERY
- (BOOL)executeUpdateWithFormat:(NSString *)format, ...;
#endif
- (BOOL)executeUpdate:(NSString*)sql withArgumentsInArray:(NSArray *)arguments;
- (BOOL)executeUpdate:(NSString*)sql error:(NSError**)outErr withArgumentsInArray:(NSArray*)arrayArgs orVAList:(va_list)args; // you shouldn't ever need to call this.  use the previous two instead.

- (FMResultSet *)executeQuery:(NSString*)sql, ...;
#ifdef ENABLE_FORMATTED_QUERY
- (FMResultSet *)executeQueryWithFormat:(NSString*)format, ...;
#endif
- (FMResultSet *)executeQuery:(NSString *)sql withArgumentsInArray:(NSArray *)arguments;
- (FMResultSet *)executeQuery:(NSString *)sql withArgumentsInArray:(NSArray*)arrayArgs orVAList:(va_list)args; // you shouldn't ever need to call this.  use the previous two instead.

#if 0
- (BOOL)rollback;
- (BOOL)commit;
- (BOOL)beginTransaction;
- (BOOL)beginDeferredTransaction;
#endif

- (BOOL)inUse;
- (void)setInUse:(BOOL)value;


- (BOOL)shouldCacheStatements;
- (void)setShouldCacheStatements:(BOOL)value;

+ (BOOL)isThreadSafe;
+ (NSString*)sqliteLibVersion;

- (int)changes;

@end

@interface FMStatement : NSObject {
    sqlite3_stmt *statement;
    NSString *query;
    long useCount;
}

@property (assign) long useCount;
@property (retain) NSString *query;
@property (assign) sqlite3_stmt *statement;

- (void)close;
- (void)reset;

- (NSString*)explainQueryPlan;
+ (NSString*)explainQueryPlan: (sqlite3_stmt*)statement;

@end

