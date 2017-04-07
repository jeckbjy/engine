using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Text;
using System.IO;
using Excel;


namespace csvgen
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                CsvConvert csv = new CsvConvert();
                if(args.Length > 0 && args[0] == "-h")
                {
                    csv.help();
                }
                else
                {
                    csv.init(args);
                    csv.process();
                }
            }
            catch(Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
    }

    /*
     * 将xls,xlsx转化为csv
     * 校验表格是否正确：1：首列不能重复
     * usage
     */
    class CsvConvert
    {
        string m_srcPath = "./";
        string m_dstPath = "./";
        string m_ext = ".csv";
        string m_lineCode = "\n";
        char m_split = ',';         // 分隔符
        bool m_utf8 = true;
        bool m_bom = false;         // utf8编码时是否带bom头
        bool m_force = true;
        bool m_check = false;
        bool m_winLine = false;     // \r\n换行否则用\n换行

        public void init(string[] args)
        {
            for(int i = 0; i < args.Length; ++i)
            {
                switch(args[i])
                {
                    case "-s":
                        {
                            ++i;
                            m_srcPath = args[i];
                        }
                        break;
                    case "-d":
                        {
                            ++i;
                            m_dstPath = args[i];
                            if(!m_dstPath.EndsWith("/"))
                            {
                                m_dstPath = m_dstPath + "/";
                            }
                        }
                        break;
                    case "-e":
                        {
                            ++i;
                            m_ext = args[i];
                            if(!m_ext.StartsWith("."))
                            {
                                m_ext = "." + m_ext;
                            }
                        }
                        break;
                    case "-u":
                        {
                            ++i;
                            m_utf8 = bool.Parse(args[i]);
                        }
                        break;
                    case "-b":
                        {
                            ++i;
                            m_bom = bool.Parse(args[i]);
                        }
                        break;
                    case "-f":
                        {
                            ++i;
                            m_force = bool.Parse(args[i]);
                        }
                        break;
                    case "-c":
                        {
                            ++i;
                            m_check = bool.Parse(args[i]);
                        }
                        break;
                    case "-w":
                        {
                            ++i;
                            m_winLine = bool.Parse(args[i]);
                            if (m_winLine)
                                m_lineCode = "\r\n";
                            else
                                m_lineCode = "\n";
                        }
                        break;
                    case "-q":
                        {
                            ++i;
                            string quote = args[i];
                            m_split = change(quote);
                        }
                        break;
                }
            }
        }

        public void help()
        {
            StringBuilder builder = new StringBuilder();
            builder.AppendLine("usage: csvgen.exe -s ./src -d ./src -e txt -q , -b true -f true -c true");
            builder.AppendLine("    -s[./   ]: src folder!");
            builder.AppendLine("    -d[./   ]: dst folder!");
            builder.AppendLine("    -e[.csv ]: extension of output file!!");
            builder.AppendLine("    -u[true ]: utf8 encoding");
            builder.AppendLine("    -b[false]: utf8 with bom");
            builder.AppendLine("    -f[true ]: force update table when not change!");
            builder.AppendLine("    -c[false]: check that first column must unique!!");
            builder.AppendLine("    -q[','  ]: split char");
            string help_str = builder.ToString();
            Console.Write(help_str);
            Console.Read();
        }

        public char change(string quote)
        {
            if(quote.Length == 0)
                return m_split;

            if(quote.Length == 1)
                return quote[0];

            if (quote == @"\t")
                return '\t';

            return m_split;
        }

        public void process()
        {
            string ext = Path.GetExtension(m_srcPath).ToLower();
            if(string.IsNullOrEmpty(ext))
            {// 遍历目录
                DirectoryInfo folder = new DirectoryInfo(m_srcPath);
                foreach (FileInfo file in folder.GetFiles("*.xls?"))
                {
                    convert(file.FullName);
                }
            }
            else
            {
                if (!File.Exists(m_srcPath))
                    throw new FileNotFoundException(m_srcPath);

                convert(m_srcPath);
            }
        }

        public void convert(string file)
        {
            string dstFile = m_dstPath + Path.GetFileNameWithoutExtension(file) + m_ext;

            if(!m_force)
            {
                DateTime srcTime = File.GetLastWriteTimeUtc(file);
                DateTime dstTime = File.GetLastWriteTimeUtc(dstFile);
                if(dstTime > srcTime)
                {
                    Console.WriteLine("file {0} is not change!!", file);
                    return;
                }
            }

            Console.WriteLine("convert file:{0}", file);

            FileStream stream = File.Open(file, FileMode.Open, FileAccess.Read);
            IExcelDataReader reader;
            string ext = Path.GetExtension(file).ToLower();
            if(ext == ".xlsx")
            {
                reader = ExcelReaderFactory.CreateOpenXmlReader(stream);
            }
            else if( ext == ".xls")
            {
                reader = ExcelReaderFactory.CreateBinaryReader(stream);
            }
            else
            {
                stream.Close();
                return;
            }

            DataSet dataset = reader.AsDataSet();

            if (dataset == null)
                throw new Exception("bad excel:" + file);

            StringBuilder builder = new StringBuilder();

            if (dataset.Tables.Count != 0)
            {
                if (m_check)
                    check(dataset, file);

                int row_count = 0;
                int col_count = dataset.Tables[0].Columns.Count;
                foreach (DataRow row in dataset.Tables[0].Rows)
                {
                    if(row_count != 0)
                        builder.Append(m_lineCode);

                    for (int i = 0; i < col_count; ++i)
                    {
                        if (i != 0)
                            builder.Append(m_split);
                        string cell = row[i].ToString();
                        if (cell.IndexOf(m_split) != -1)
                            builder.Append(string.Format("\"{0}\"", cell));
                        else
                            builder.Append(cell);
                    }

                    ++row_count;
                }
            }

            // 写文件
            string csvData = builder.ToString();

            Encoding encoding;
            if(m_utf8)
            {
                encoding = new UTF8Encoding(m_bom);
            }
            else
            {
                encoding = Encoding.Default;
            }

            File.WriteAllText(dstFile, csvData, encoding);
        }

        private void check(DataSet dataset, string file)
        {
            HashSet<long> IDSet = new HashSet<long>();
            int row_count = dataset.Tables[0].Rows.Count;
            DataColumn col0 = dataset.Tables[0].Columns[0];
            // 前三行忽略
            for(int i = 3; i < row_count; ++i)
            {
                string cell = dataset.Tables[0].Rows[i][0].ToString();
                if (string.IsNullOrEmpty(cell))
                    continue;
                if (cell[0] == '#')
                    continue;
                long uid = long.Parse(cell);
                if (IDSet.Contains(uid))
                    throw new Exception(string.Format("duplateID:{0} in {1}", uid, Path.GetFileName(file)));

                IDSet.Add(uid);
            }
        }
    }
}
