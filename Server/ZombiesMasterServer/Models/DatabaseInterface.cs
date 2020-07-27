using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Web;

namespace ZombiesMasterServer.Models
{
    public class DatabaseInterface
    {
        private MySqlConnection SqlConnection;
        private string ConnectionString;

        public DatabaseInterface()
        {
            ConnectionString = ConfigurationManager.ConnectionStrings["WorldAtWarConnectionString"].ConnectionString;
            SqlConnection = new MySqlConnection(ConnectionString);
        }

        public int PostData(ServerData data)
        {
            try
            {
                SqlConnection.Open();

                MySqlCommand command = new MySqlCommand("AddServerEntry", SqlConnection);
                command.CommandType = System.Data.CommandType.StoredProcedure;
                command.Parameters.AddWithValue("_ServerID", data.ServerID);
                command.Parameters.AddWithValue("_IPAddress", data.IPAddress);
                command.Parameters.AddWithValue("_ServerName", data.ServerName);
                command.Parameters.AddWithValue("_MapName", data.MapName);
                command.Parameters.AddWithValue("_CurrentPlayers", data.CurrentPlayers);
                command.Parameters.AddWithValue("_MaxPlayers", data.MaxPlayers);

                command.ExecuteNonQuery();

                SqlConnection.Close();

                return 999;
            }
            catch (Exception ex)
            {
                return -1;
            }
        }
    }
}