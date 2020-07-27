using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using MySql.Data.MySqlClient;
using System.Configuration;

namespace ZombiesMasterServer.Models
{
    public class ServerData
    {
        public int ServerID { get; set; }
        public string IPAddress { get; set; }
        public string ServerName { get; set; }
        public string MapName { get; set; }
        public int CurrentPlayers { get; set; }
        public int MaxPlayers { get; set; }

        public void Test()
        {
            string connectionString = ConfigurationManager.ConnectionStrings["WorldAtWarConnectionString"].ConnectionString;

            MySqlConnection sqlConnection = new MySqlConnection(connectionString);

            try
            {
                sqlConnection.Open();

                MySqlCommand command = new MySqlCommand("AddServerEntry", sqlConnection);
                command.CommandType = System.Data.CommandType.StoredProcedure;
                command.Parameters.AddWithValue("_ServerID", 5);
                command.Parameters.AddWithValue("_IPAddress", "127.0.0.1");
                command.Parameters.AddWithValue("_ServerName", "Test Server Name");
                command.Parameters.AddWithValue("_MapName", "Default Map");
                command.Parameters.AddWithValue("_CurrentPlayers", 5);
                command.Parameters.AddWithValue("_MaxPlayers", 50);

                command.ExecuteNonQuery();

                sqlConnection.Close();
            }
            catch (Exception ex)
            {

            }
        }
    }
}