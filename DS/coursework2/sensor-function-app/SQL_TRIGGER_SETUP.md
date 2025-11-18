# SQL Trigger Setup Guide

This guide explains how to set up the SQL trigger that automatically calls `QueryStatsDB` function when data is inserted into the `sensors` table.

## How It Works

1. **DataFunction** (Timer Trigger): Runs every 10 seconds and inserts 20 sensor readings into the database
2. **SQL Database Trigger**: Automatically fires when data is inserted into the `sensors` table
3. **QueryStatsDB** (HTTP Trigger): Called by the SQL trigger via HTTP endpoint with function key authentication

## Setup Steps

### Step 1: Get Your Function Key

You need the function key to authenticate the HTTP call from the SQL trigger.

**Option A: Via Azure Portal**
1. Go to Azure Portal → Your Function App (`myfunctionapp14`)
2. Navigate to **Functions** → **QueryStatsDB**
3. Click **Function Keys**
4. Copy the **default** key (or create a new one and copy it)

**Option B: Via Azure CLI**
```bash
az functionapp function keys list \
  --resource-group java-functions-group \
  --name myfunctionapp14 \
  --function-name QueryStatsDB \
  --query "default" \
  --output tsv
```

### Step 2: Update the SQL Trigger Script

1. Open `sql_trigger_setup.sql`
2. Replace `YOUR_FUNCTION_KEY_HERE` with the actual function key you copied
3. Verify the Function App URL is correct (should be: `https://myfunctionapp14-a0e6a6gra0dhbha9.francecentral-01.azurewebsites.net`)

### Step 3: Create the SQL Trigger

Run the SQL script in your Azure SQL Database:

```bash
# Connect to your Azure SQL Database
# Then run the sql_trigger_setup.sql script
```

Or via Azure CLI:
```bash
az sql db execute \
  --resource-group YOUR_RESOURCE_GROUP \
  --server YOUR_SQL_SERVER \
  --database YOUR_DATABASE \
  --file-path sql_trigger_setup.sql
```

### Step 4: Verify the Trigger Works

1. The trigger fires automatically when 20+ rows are inserted into the `sensors` table
2. Check Azure Function App logs to see if QueryStatsDB is being called
3. The trigger only fires when 20 or more rows are inserted (complete batch)

## SQL Trigger Details

- **Trigger Name**: `trg_QueryStatsAfterInsert`
- **Table**: `sensors`
- **Event**: `AFTER INSERT`
- **Condition**: Only triggers when 20+ rows inserted (complete batch)
- **Action**: Calls QueryStatsDB HTTP endpoint using `sp_invoke_external_rest_endpoint`

## Function Authentication

The QueryStatsDB function uses `AuthorizationLevel.FUNCTION`, which requires:
- Function key in URL: `/api/QueryStatsDB?code=YOUR_FUNCTION_KEY`
- The SQL trigger includes this key automatically in the URL

## Troubleshooting

### Function Not Being Called

1. **Check Function Key**: Verify the key in `sql_trigger_setup.sql` matches the actual function key
2. **Check Function App URL**: Ensure it's correct
3. **Check SQL Logs**: The trigger logs errors if the HTTP call fails
4. **Check Function App Logs**: Look for incoming requests to QueryStatsDB

### Trigger Not Firing

1. **Verify Trigger Exists**: Run this in SQL:
   ```sql
   SELECT name, is_disabled FROM sys.triggers WHERE parent_id = OBJECT_ID('sensors');
   ```
2. **Check if Trigger is Disabled**: 
   ```sql
   ALTER TABLE sensors ENABLE TRIGGER trg_QueryStatsAfterInsert;
   ```
3. **Test Manually**: Try inserting 20 rows and check if trigger fires

### sp_invoke_external_rest_endpoint Not Available

If `sp_invoke_external_rest_endpoint` is not available in your Azure SQL Database tier:
- Upgrade to a tier that supports it (typically available in most Azure SQL Database tiers)
- Or use alternative approaches like Azure Logic Apps or Event Grid

## Managing the Trigger

**Disable the trigger:**
```sql
ALTER TABLE sensors DISABLE TRIGGER trg_QueryStatsAfterInsert;
```

**Re-enable the trigger:**
```sql
ALTER TABLE sensors ENABLE TRIGGER trg_QueryStatsAfterInsert;
```

**Drop the trigger:**
```sql
DROP TRIGGER trg_QueryStatsAfterInsert;
```

**View trigger status:**
```sql
SELECT name, is_disabled, create_date, modify_date 
FROM sys.triggers 
WHERE parent_id = OBJECT_ID('sensors');
```

## Security Notes

- The function key provides authentication for the HTTP endpoint
- Function keys should be kept secure and rotated periodically
- The SQL trigger stores the key in the trigger definition (visible to database administrators)
- Consider using Azure Key Vault for more secure key management in production

