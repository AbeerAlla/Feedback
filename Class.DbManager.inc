<?php
// Apply signleton design pattern in DbManager class
Class DbManager {
    private $_connection;       // the connection string 
    private static $_instance;  //static instance for the one connection
    private $_host = "localhost"; // Hosting type
    private $_username = "root"; // username for DB account
    private $_password = "";// password for DB account
    private $dbname = "test"; // DB name
    
    // getInstance method for get an object
    public static function getInstance()
    {
        if (!self::$_instance){
            self::$_instance = new self();
        }
        return self::$_instance;   
    }
    //override for the constructor 
    private function __construct() {
        $this->_connection = DbManager::openConnection($this->_host,$this->dbname,$this->_username,$this->_password);
    }
    // to prevent make a clone obj for DbManager
    private function __clone() {}

    public function login($username,$password)
    {
        try
        {
         $stat = $this->_connection->prepare(
                 "SELECT `username`, `password` ,`type` FROM `login` WHERE username = :username AND password = :password"
                 );
           $stat->bindParam(':username', $username);
           $stat->bindParam(':password', $password);
           $stat->execute();
           $result = $stat->fetchObject();
           
           if ($result)
           {
              return $result;
           }
           else 
           {echo "Invalid username or Password";}
           //echo "username :".$result->username."<br/>"."password :".$result->password;     
           
        }catch(PDOException $e)
        {
            echo "Can't login from DB" ;
        }
    }
    
    public function searchRecord($column_name,$value,$table_name)
    {
        $sql = "SELECT * FROM `$table_name` WHERE `$column_name` = '$value'";
        try{
            $stat = $this->_connection->prepare($sql);
            $stat->execute();
            $result = $stat->fetchObject();
            if ($result)
            {
                return $result;   
            }
            else
            {
                echo "The Error from DB";
            }
        }catch(PDOException $e)
        {
            $e->getMessage();
        }
    }
    
    public function searchAll ($tablename)
    {   
        $sql = "SELECT * FROM `$tablename`";
        try{
        
        $stat = $this->_connection->prepare($sql);
        $stat->execute();
        $arr = array();
        while($result = $stat->fetchObject())
        { 
             array_push($arr, $result);
        }
        return $arr;
       
        }catch (PDOException $e){
        echo 'Done' . $e->getMessage();}
        
    }
    
    //for execute the query
    public function execQuery($sql_query)
    {
        try{
           $this->_connection->exec($sql_query);
            
        }
        catch(PDOException $e)
        {
            echo $sql_query . "</br>" . $e->getMessage();
        }
    }
    //for open the PDO Connection 
    private static function openConnection($servername,$dbname,$username,$password)
    {   try{
                $_myconnection = new PDO("mysql:host=$servername;dbname=$dbname",$username,$password);
                $_myconnection->setAttribute(PDO::ATTR_ERRMODE,PDO::ERRMODE_EXCEPTION);
                //echo "Connection Successfully"."<br/>";
                return $_myconnection;
            }  
            catch (PDOException $e)
            {echo "Connection Failed" . $e->getMessage();}
            
    }
    // for closing the connection
    public  function closeConnection ()
    {   
        if ($this->_connection){
        $this->_connection = NULL;
        }
    }
    
    public function filterString ($string)
    {
        $result = filter_var($string,FILTER_SANITIZE_STRING);
        return $result;
    }
    public function validateEmail ($string)
    {
        $result = filter_var($string,FILTER_SANITIZE_EMAIL);
        if (!filter_var($result) === false)
        {return $result;}
        else {echo "Not Valid EMail";}
    }
        
    
    
}// end of DbManager class
    