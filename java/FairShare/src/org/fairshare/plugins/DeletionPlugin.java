package org.fairshare.plugins;


public class DeletionPlugin {

    // For the most general types of plugins, these methods should be considered to be "setup" methods.
    // They should be given the whole environment, or a substantial part of it,
    // and be allowed to adjust whatever settings they require.
    
    public void mutateTypes() {
        // Add a deleteOnDate type/field which may be given to Files only
    }
    
    public void mutateActions() {
        // Add a "task/job" to occur regularly (e.g. daily)
        // Naturally, it deletes any files whos deleteOnDate has expired.
    }
    
}
