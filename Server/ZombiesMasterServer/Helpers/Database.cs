using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Web;
using System.Web.Configuration;

namespace ZombiesMasterServer.Helpers
{
    public class Database
    {
        public SqlConnection connection = null;
        private static string connectionString = WebConfigurationManager.ConnectionStrings["WorldAtWarConnectionString"].ConnectionString;

        public static bool NonQuery(string sql, Dictionary<string, object> variables = null)
        {
            SqlConnection conn = null;

            try
            {
                conn = new SqlConnection(connectionString);
            }
            finally
            {
                if (conn != null)
                {
                    conn.Dispose();
                }
            }

            using (SqlCommand command = new SqlCommand(sql, conn))
            {
                if (variables != null)
                {
                    foreach (KeyValuePair<string, object> item in variables)
                    {
                        command.Parameters.AddWithValue(item.Key, item.Value);
                    }
                }

                return BaseNonQuery(command);
            }
        }

        public static bool BaseNonQuery(SqlCommand command)
        {
            command.CommandTimeout = 0;
            bool returnBool = true;
            SqlConnection connection = null;

            try
            {
                connection = new SqlConnection(connectionString);

                try
                {
                    connection.Open();
                    command.Connection = connection;
                    command.ExecuteNonQuery();
                }
                catch (Exception ex)
                {
                    string s = ex.Message;
                    returnBool = false;
                }
            }
            finally
            {
                if (connection != null)
                {
                    connection.Dispose();
                }
            }
            return returnBool;
        }

        public static DataTable BaseDataQuery(SqlCommand command)
        {
            command.CommandTimeout = 0;
            DataTable dt = null;
            SqlConnection conn = null;

            try
            {
                conn = new SqlConnection(connectionString);

                try
                {
                    conn.Open();
                    command.Connection = conn;
                    SqlDataAdapter adapter = new SqlDataAdapter(command);
                    dt = new DataTable();
                    adapter.Fill(dt);
                }
                catch (Exception ex)
                {
                    //InsertErrorLog("Failed a WPMDataQuery", ex);
                    dt = null;
                }
            }
            finally
            {
                if (conn != null)
                {
                    conn.Dispose();
                }
            }

            return dt;
        }

    }
}