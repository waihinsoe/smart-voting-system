import { Table, Tag } from "antd";
import type { TableProps } from "antd";
import { useContext } from "react";
import { AppContext } from "../../context/appContext";
import LoadingPhoto from "../../assets/loadingPhoto.svg";

interface DataType {
  key: string;
  name: string;
  position: string;
  qualification: string;
}

interface Voter {
  name: string;
  position: string;
  qualification: string;
}

// Define the type for the voters object where each property is a Voter
interface Voters {
  [key: string]: Voter;
}

const columns: TableProps<DataType>["columns"] = [
  {
    title: "ID",
    dataIndex: "key",
    key: "key",
    // render: (text) => <a>{text}</a>,
  },
  {
    title: "Name",
    dataIndex: "name",
    key: "name",
  },
  {
    title: "position",
    dataIndex: "position",
    key: "position",
  },
  {
    title: "qualification",
    key: "qualification",
    dataIndex: "qualification",
    render: (qualification) => (
      <Tag color="green" key={qualification}>
        {qualification.toUpperCase()}
      </Tag>
    ),
  },
];

export const Voters = () => {
  const { voters }: { voters: Voters } = useContext(AppContext);

  if (!voters)
    return (
      <div style={{ width: "100%", display: "grid", placeItems: "center" }}>
        <img src={LoadingPhoto} alt="loading..." />
      </div>
    );
  const formattedVoters = Object.entries(voters).map(
    ([id, { name, position, qualification }]: [string, Voter]) => {
      return {
        key: id,
        name: name,
        position: position,
        qualification: qualification,
      };
    }
  );

  return (
    <div style={{ width: "100%", maxWidth: 900, margin: "0 auto " }}>
      <Table
        columns={columns}
        dataSource={formattedVoters}
        scroll={{ x: 600 }}
      />
    </div>
  );
};
