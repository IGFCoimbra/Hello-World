import React from 'react';
import { Helmet } from 'react-helmet';

const AccountEditor = () => {
    return (
        <React.Fragment>
            <Helmet>
                <title>Account Editor</title>
            </Helmet>
            <div>This is the account editor.</div>
        </React.Fragment>
    );
};

AccountEditor.displayName = 'Account Editor';

export default AccountEditor;