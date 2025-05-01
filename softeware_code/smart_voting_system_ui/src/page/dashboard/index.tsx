import { Tabs } from "antd";
import type { TabsProps } from "antd";
import { Candidates } from "./Candidates";
import { Voters } from "./Voters";
import { VoteResult } from "./VoteResult";
import { useState } from "react";

const items: TabsProps["items"] = [
  {
    key: "voteResult",
    label: "VoteResult",
    children: <VoteResult />,
  },
  {
    key: "candidates",
    label: "Candidates",
    children: <Candidates />,
  },
  {
    key: "voters",
    label: "Voters",
    children: <Voters />,
  },
];

export const DashboardPage = () => {
  const [tabKey, setTabKey] = useState(
    sessionStorage.getItem("tabKey") || "voteResult"
  );
  const onChange = (key: string) => {
    console.log(key);
    setTabKey(key);
    sessionStorage.setItem("tabKey", key);
  };
  return <Tabs activeKey={tabKey} items={items} centered onChange={onChange} />;
};
