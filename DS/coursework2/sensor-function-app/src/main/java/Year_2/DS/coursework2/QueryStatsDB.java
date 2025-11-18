package Year_2.DS.coursework2;

import com.microsoft.azure.functions.*;
import com.microsoft.azure.functions.annotation.*;

import java.io.IOException;
import java.sql.*;
import java.time.LocalDateTime;
import java.util.Optional;

/**
 * Azure Function for querying sensor statistics from the database.
 * Triggered automatically by SQL database trigger when data is inserted into the sensors table.
 * The SQL trigger calls this HTTP endpoint using sp_invoke_external_rest_endpoint.
 * 
 * Authentication: Uses FUNCTION level auth, requires function key in URL parameter.
 * The SQL trigger passes the function key as: /api/QueryStatsDB?code=YOUR_FUNCTION_KEY
 */
public class QueryStatsDB {

    /**
     * Azure Function that queries sensor statistics.
     * Automatically triggered by SQL database trigger when data is inserted.
     * The SQL trigger in the database calls this HTTP endpoint when inserts occur.
     * 
     * This function is called by the SQL trigger: trg_QueryStatsAfterInsert
     * which uses sp_invoke_external_rest_endpoint to call this endpoint.
     */
    @FunctionName("QueryStatsDB")
    public void run(
            @HttpTrigger(
                name = "req",
                methods = {HttpMethod.POST, HttpMethod.GET},
                authLevel = AuthorizationLevel.FUNCTION) // Requires function key in URL
            HttpRequestMessage<Optional<String>> request,
            final ExecutionContext context) {
        
        context.getLogger().info("QueryStatsDB triggered by SQL database change at: " + LocalDateTime.now());
        context.getLogger().info("Database modification detected - querying sensor statistics.");

        try (Connection connection = getConnection(context)) {
            context.getLogger().info("Database connection established.");

            StringBuilder result = new StringBuilder();
            result.append("Sensor Statistics Query Results\n");
            result.append("================================\n");
            result.append("Timestamp: ").append(LocalDateTime.now()).append("\n\n");

            // Execute all queries to get complete statistics
            result.append(getTempStats(connection, context));
            result.append("\n");
            result.append(getWindStats(connection, context));
            result.append("\n");
            result.append(getHumidityStats(connection, context));
            result.append("\n");
            result.append(getCO2Stats(connection, context));

            context.getLogger().info("Successfully retrieved all statistics.");
            context.getLogger().info(result.toString());

        } catch (Exception e) {
            context.getLogger().severe("Error querying database: " + e.getMessage());
            e.printStackTrace();
        }
    }

    /**
     * Helper method to get a database connection.
     * Uses environment variables set in the Function App's configuration.
     */
    private Connection getConnection(ExecutionContext context) throws SQLException, IOException {
        String dbServer = System.getenv("DB_SERVER");
        String dbName = System.getenv("DB_NAME");
        String dbUser = System.getenv("DB_USER");
        String dbPassword = System.getenv("DB_PASSWORD");

        if (dbServer == null || dbName == null || dbUser == null || dbPassword == null) {
            throw new SQLException("Database credentials are not in environment variables. Please set DB_SERVER, DB_NAME, DB_USER, and DB_PASSWORD.");
        }

        String connectionUrl = String.format(
            "jdbc:sqlserver://%s.database.windows.net:1433;database=%s;user=%s@%s;password=%s;encrypt=true;trustServerCertificate=false;hostNameInCertificate=*.database.windows.net;loginTimeout=30;",
            dbServer, dbName, dbUser, dbServer, dbPassword
        );

        Connection connection = DriverManager.getConnection(connectionUrl);
        return connection;
    }

    /**
     * Retrieves temperature statistics and returns as formatted string.
     * Groups sensor_id values into groups of 20 (1-20 = Sensor 1, 21-40 = Sensor 2, etc.)
     */
    private String getTempStats(Connection database, ExecutionContext context) throws SQLException {
        StringBuilder result = new StringBuilder();
        result.append("Temperature Stats:\n");
        
        try (Statement statement = database.createStatement();
             ResultSet results = statement.executeQuery(
                 "SELECT ((sensor_id - 1) / 20) + 1 AS sensor_number, " +
                 "MIN(temperature) AS min_temp, MAX(temperature) AS max_temp, AVG(temperature) AS avg_temp " +
                 "FROM sensors GROUP BY ((sensor_id - 1) / 20) + 1 ORDER BY sensor_number")) {
            
            while (results.next()) {
                int sensorNumber = results.getInt("sensor_number");
                int minTemp = results.getInt("min_temp");
                int maxTemp = results.getInt("max_temp");
                float avgTemp = results.getFloat("avg_temp");
                result.append(String.format("Sensor %-2d | Max=%-2d | Min=%-2d | Avg=%.2f%n", 
                    sensorNumber, maxTemp, minTemp, avgTemp));
            }
        }
        return result.toString();
    }

    /**
     * Retrieves wind speed statistics and returns as formatted string.
     * Groups sensor_id values into groups of 20 (1-20 = Sensor 1, 21-40 = Sensor 2, etc.)
     */
    private String getWindStats(Connection database, ExecutionContext context) throws SQLException {
        StringBuilder result = new StringBuilder();
        result.append("Wind Speed Stats:\n");
        
        try (Statement statement = database.createStatement();
             ResultSet results = statement.executeQuery(
                 "SELECT ((sensor_id - 1) / 20) + 1 AS sensor_number, " +
                 "MIN(windspeed) AS min_wind, MAX(windspeed) AS max_wind, AVG(windspeed) AS avg_wind " +
                 "FROM sensors GROUP BY ((sensor_id - 1) / 20) + 1 ORDER BY sensor_number")) {
            
            while (results.next()) {
                int sensorNumber = results.getInt("sensor_number");
                int minWind = results.getInt("min_wind");
                int maxWind = results.getInt("max_wind");
                float avgWind = results.getFloat("avg_wind");
                result.append(String.format("Sensor %-2d | Max=%-2d | Min=%-2d | Avg=%.2f%n", 
                    sensorNumber, maxWind, minWind, avgWind));
            }
        }
        return result.toString();
    }

    /**
     * Retrieves humidity statistics and returns as formatted string.
     * Groups sensor_id values into groups of 20 (1-20 = Sensor 1, 21-40 = Sensor 2, etc.)
     */
    private String getHumidityStats(Connection database, ExecutionContext context) throws SQLException {
        StringBuilder result = new StringBuilder();
        result.append("Relative Humidity Stats:\n");
        
        try (Statement statement = database.createStatement();
             ResultSet results = statement.executeQuery(
                 "SELECT ((sensor_id - 1) / 20) + 1 AS sensor_number, " +
                 "MIN(relative_humidity) AS min_humidity, MAX(relative_humidity) AS max_humidity, AVG(relative_humidity) AS avg_humidity " +
                 "FROM sensors GROUP BY ((sensor_id - 1) / 20) + 1 ORDER BY sensor_number")) {
            
            while (results.next()) {
                int sensorNumber = results.getInt("sensor_number");
                int minHumidity = results.getInt("min_humidity");
                int maxHumidity = results.getInt("max_humidity");
                float avgHumidity = results.getFloat("avg_humidity");
                result.append(String.format("Sensor %-2d | Max=%-2d | Min=%-2d | Avg=%.2f%n", 
                    sensorNumber, maxHumidity, minHumidity, avgHumidity));
            }
        }
        return result.toString();
    }

    /**
     * Retrieves CO2 statistics and returns as formatted string.
     * Groups sensor_id values into groups of 20 (1-20 = Sensor 1, 21-40 = Sensor 2, etc.)
     */
    private String getCO2Stats(Connection database, ExecutionContext context) throws SQLException {
        StringBuilder result = new StringBuilder();
        result.append("CO2 Stats:\n");
        
        try (Statement statement = database.createStatement();
             ResultSet results = statement.executeQuery(
                 "SELECT ((sensor_id - 1) / 20) + 1 AS sensor_number, " +
                 "MIN(CO2) AS min_co2, MAX(CO2) AS max_co2, AVG(CO2) AS avg_co2 " +
                 "FROM sensors GROUP BY ((sensor_id - 1) / 20) + 1 ORDER BY sensor_number")) {
            
            while (results.next()) {
                int sensorNumber = results.getInt("sensor_number");
                int minCO2 = results.getInt("min_co2");
                int maxCO2 = results.getInt("max_co2");
                float avgCO2 = results.getFloat("avg_co2");
                result.append(String.format("Sensor %-2d | Max=%-2d | Min=%-2d | Avg=%.2f%n", 
                    sensorNumber, maxCO2, minCO2, avgCO2));
            }
        }
        return result.toString();
    }
}
