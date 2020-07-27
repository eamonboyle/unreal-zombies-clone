using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;
using ZombiesMasterServer.Models;

namespace ZombiesMasterServer.Controllers
{
    public class HostController : ApiController
    {
        // GET: api/Host
        public IEnumerable<string> Get()
        {
            return new string[] { "value1", "value2" };
        }

        // GET: api/Host/5
        public string Get(int id)
        {
            return "value";
        }

        // POST: api/Host
        public int Post(ServerData data)
        {
            DatabaseInterface DB = new DatabaseInterface();
            return DB.PostData(data);
        }

        // PUT: api/Host/5
        public void Put(int id, [FromBody]string value)
        {
        }

        // DELETE: api/Host/5
        public void Delete(int id)
        {
        }
    }
}
